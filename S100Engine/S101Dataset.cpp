#include "stdafx.h"
#include "S101Dataset.h"
#include "PointInstruction.h"
#include "LineInstruction.h"
#include "AreaInstruction.h"
#include "TextInstruction.h"
#include "AugmentedPath.h"
#include "AugmentedRay.h"
#include "S100DrawingUnitPoint.h"
#include "S100DrawingUnitPolyLine.h"
#include "S100DrawingUnitPolygon.h"
#include "S100DrawingUnitPatternPolygon.h"
#include "S100DrawingUnitTextPoint.h"
#include "S100DrawingUnitAugmentedPath.h"
#include "S100DrawingUnitAugmentedRay.h"
#include "S100PCManager.h"
#include "TextPoint.h"
#include "TextElement.h"


S101Dataset::S101Dataset()
{

}

S101Dataset::~S101Dataset()
{
	for (auto i = polyLineMap.begin(); i != polyLineMap.end(); i++)
	{
		SafeRelease(&(i->second));
	}

	for (auto i = polygonMap.begin(); i != polygonMap.end(); i++)
	{
		SafeRelease(&(i->second));
	}

	delete displayList;
}

CString S101Dataset::GetFilePath()
{
	return filePath;
}

bool S101Dataset::OpenS101SENC(CString _path)
{
	filePath = _path;
	return senc.Open(_path);
}

bool S101Dataset::OpenS101(CString _path)
{
	bool ret = false;
	ret = base.LoadBaseCell(_path);
	fileName = LibMFCUtil::GetFileName(_path);
	usage = GetUsageFromFileName();
	base.GetBoundingBox(xmin, ymin, xmax, ymax, scaleMin, scaleMax);
	filePath = _path;
	return ret;
}

ID2D1PathGeometry* S101Dataset::CreatePolylineGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	int featureID = pFeatureRecord->stFRID.nRCID;
	ID2D1PathGeometry* pGeometry = nullptr;

	if (base.GetGeometryTypeFromFeatureRecord(featureID) == 120)
	{
		pGeometry = CreatePolylineGeometryFromCurveFeature(pDirect2dFactory, pFeatureRecord);
	}
	else if (base.GetGeometryTypeFromFeatureRecord(featureID) == 125)
	{
		pGeometry = CreatePolylineGeometryFromCompositeCurveFeature(pDirect2dFactory, pFeatureRecord);
	}
	else if (base.GetGeometryTypeFromFeatureRecord(featureID) == 130)
	{
		pGeometry = CreatePolylineGeometryFromSurfaceFeature(pDirect2dFactory, pFeatureRecord);
	}

	return pGeometry;
}


ID2D1PathGeometry* S101Dataset::CreatePolylineGeometryFromCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	int featureID = pFeatureRecord->stFRID.nRCID;
	int curveID = pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID;
	ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(curveID);

	std::vector<D2D1_POINT_2F> points;

	GetPoints(pCurveRecord, points);

	if (points.size() < 2)
	{
		OutputDebugString(_T("Faield to GetPoints\n"));
		return nullptr;
	}

	ID2D1PathGeometry* pGeometry = nullptr;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink *pSink = nullptr;

		hr = pGeometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
			pSink->BeginFigure(*points.begin(), D2D1_FIGURE_BEGIN_FILLED);

			for (auto i = points.begin() + 1; i != points.end(); i++)
			{
				pSink->AddLine(*i);
			}

			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

			hr = pSink->Close();

			if (SUCCEEDED(hr))
			{
				return pGeometry;
			}
			else
			{
				OutputDebugString(_T("Faield to close sink\n"));
			}
		}
		else
		{
			OutputDebugString(_T("Faield to Open Sink\n"));
		}
	}
	else
	{
		OutputDebugString(_T("Failed to create direct2d geometry\n"));
	}
	return nullptr;
}

ID2D1PathGeometry* S101Dataset::CreatePolylineGeometryFromCompositeCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	int featureID = pFeatureRecord->stFRID.nRCID;
	int compCurveID = pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID;
	ST_COMP_CURVE_RECORD* pCompCurveRecord = base.GetCompositeCurveRecord(compCurveID);

	std::vector<D2D1_POINT_2F> points;

	GetPoints(pCompCurveRecord, points);

	if (points.size() < 2)
	{
		OutputDebugString(_T("Faield to GetPoints\n"));
		return nullptr;
	}

	ID2D1PathGeometry* pGeometry = nullptr;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink *pSink = nullptr;

		hr = pGeometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
			pSink->BeginFigure(*points.begin(), D2D1_FIGURE_BEGIN_FILLED);

			for (auto i = points.begin() + 1; i != points.end(); i++)
			{
				pSink->AddLine(*i);
			}

			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

			hr = pSink->Close();

			if (SUCCEEDED(hr))
			{
				return pGeometry;
			}
			else
			{
				OutputDebugString(_T("Faield to close sink\n"));
			}
		}
		else
		{
			OutputDebugString(_T("Faield to Open Sink\n"));
		}
	}
	else
	{
		OutputDebugString(_T("Failed to create direct2d geometry\n"));
	}

	return nullptr;
}

ID2D1PathGeometry* S101Dataset::CreatePolylineGeometryFromSurfaceFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	return nullptr;
}

ID2D1PathGeometry* S101Dataset::CreatePolygonGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1PathGeometry* pGeometry = nullptr;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

	if (SUCCEEDED(hr))
	{
		std::vector<ID2D1PathGeometry*> m_vecRing;

		if (pFeatureRecord)
		{
			ST_SURFACE_RECORD* pSurfaceRecord = base.GetSurfaceRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

			if (pSurfaceRecord)
			{
				for (auto i = pSurfaceRecord->vecRIAS.begin(); i != pSurfaceRecord->vecRIAS.end(); i++)
				{
					for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
					{
						ID2D1PathGeometry* pRingGeometry = nullptr;
						std::vector<D2D1_POINT_2F> points;

						if (j->nRRNM == 120)
						{
							ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(j->nRRID);
							if (pCurveRecord)
							{
								GetPoints(pCurveRecord, points);
							}
						}
						else if (j->nRRNM == 125)
						{
							ST_COMP_CURVE_RECORD* pCompCurveRecord = base.GetCompositeCurveRecord(j->nRRID);
							if (pCompCurveRecord)
							{
								GetPoints(pCompCurveRecord, points);
							}
						}
						else
						{
							OutputDebugString(_T("Invalied RRNM in RIAS\n"));
						}

						if (points.size() > 0)
						{
							hr = pDirect2dFactory->CreatePathGeometry(&pRingGeometry);

							if (SUCCEEDED(hr))
							{
								ID2D1GeometrySink *pSink = nullptr;

								hr = pRingGeometry->Open(&pSink);

								if (SUCCEEDED(hr))
								{
									pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
									pSink->BeginFigure(
										*points.begin(),
										D2D1_FIGURE_BEGIN_FILLED
										);

									for (auto k = points.begin() + 1; k != points.end(); k++)
									{
										pSink->AddLine(*k);
									}

									pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

									hr = pSink->Close();

									if (SUCCEEDED(hr))
									{
										m_vecRing.push_back(pRingGeometry);
									}
									else
									{
										OutputDebugString(_T("Faield to close ring sink\n"));
									}
								}
								else
								{
									OutputDebugString(_T("Faield to Open Ring Sink\n"));
								}
							}
							else
							{
								OutputDebugString(_T("Failed to create ring geometry\n"));
							}
						}
					}
				}
			}
		}

		ID2D1GeometrySink* pSink = nullptr;

		if (SUCCEEDED(hr))
		{
			if (m_vecRing.size() > 1)
			{
				hr = pGeometry->Open(&pSink);

				ID2D1PathGeometry* pRingGeometry1 = *m_vecRing.begin();
				ID2D1PathGeometry* pRingGeometry2 = *(m_vecRing.begin() + 1);

				hr = pRingGeometry1->CombineWithGeometry(pRingGeometry2, D2D1_COMBINE_MODE_XOR, NULL, NULL, pSink);

				pSink->Close();

				for (int i = 2; i < (int)m_vecRing.size(); i++)
				{
					ID2D1PathGeometry *pCurrentGeom = nullptr;
					pDirect2dFactory->CreatePathGeometry(&pCurrentGeom);
					ID2D1GeometrySink *pCurrentSink = nullptr;
					pCurrentGeom->Open(&pCurrentSink);

					pGeometry->CombineWithGeometry(m_vecRing.at(i), D2D1_COMBINE_MODE_XOR, NULL, NULL, pCurrentSink);

					pCurrentSink->Close();
					pGeometry = pCurrentGeom;
				}

				pGeometry = *m_vecRing.begin();
			}
			else if (m_vecRing.size() == 1)
			{
				pGeometry = *m_vecRing.begin();
			}
		}
		else
		{
			OutputDebugString(_T("Faield to open sink\n"));
		}
	}
	else
	{
		OutputDebugString(_T("Failed to create geometry\n"));
	}

	return pGeometry;
}

void S101Dataset::GetPoints(ST_CURVE_RECORD* pST_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points)
{
	D2D1_POINT_2F* pBeginningPt = nullptr;
	D2D1_POINT_2F* pEndPt = nullptr;
	D2D1_POINT_2F* pBeginningAndEndPt = nullptr;

	for (auto j = pST_CURVE_RECORD->vecPTAS.begin(); j != pST_CURVE_RECORD->vecPTAS.end(); j++)
	{
		if (j->nTOPI == 1)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = base.GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pBeginningPt = new D2D1_POINT_2F;

			pBeginningPt->x = (FLOAT)x;
			pBeginningPt->y = (FLOAT)y;
		}
		else if (j->nTOPI == 2)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = base.GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pEndPt = new D2D1_POINT_2F;

			pEndPt->x = (FLOAT)x;
			pEndPt->y = (FLOAT)y;
		}
		else if (j->nTOPI == 3)
		{
			double x = 0;
			double y = 0;

			ST_POINT_RECORD* pPointRecord = base.GetPointRecord(j->nRRID);
			if (pPointRecord)
			{
				x = pPointRecord->stC2IT.nXCOO / 10000000.0;
				y = -pPointRecord->stC2IT.nYCOO / 10000000.0;
			}

			pBeginningAndEndPt = new D2D1_POINT_2F;

			pBeginningAndEndPt->x = (FLOAT)x;
			pBeginningAndEndPt->y = (FLOAT)y;
		}
	}

	if (pBeginningPt)
	{
		points.push_back(*pBeginningPt);
	}

	if (pBeginningAndEndPt)
	{
		points.push_back(*pBeginningAndEndPt);
	}

	for (auto j = pST_CURVE_RECORD->vecC2IL.begin(); j != pST_CURVE_RECORD->vecC2IL.end(); j++)
	{
		double x = j->nXCOO / 10000000.0;
		double y = -j->nYCOO / 10000000.0;

		D2D1_POINT_2F pt;
		pt.x = (FLOAT)x;
		pt.y = (FLOAT)y;

		points.push_back(pt);
	}

	if (pEndPt)
	{
		points.push_back(*pEndPt);
	}

	if (pBeginningAndEndPt)
	{
		points.push_back(*pBeginningAndEndPt);
	}

	delete pBeginningPt;
	delete pBeginningAndEndPt;
	delete pEndPt;
}

void S101Dataset::GetPoints(ST_COMP_CURVE_RECORD* pST_COMP_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points)
{
	for (auto i = pST_COMP_CURVE_RECORD->vecCUCO.begin(); i != pST_COMP_CURVE_RECORD->vecCUCO.end(); i++)
	{
		for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
		{
			if (j->nRRNM == 120)
			{
				ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(j->nRRID);

				if (pCurveRecord)
				{
					std::vector<D2D1_POINT_2F> subPoints;
					GetPoints(pCurveRecord, subPoints);

					if (subPoints.size() > 0)
					{
						if (j->nORNT == 1)
						{
							for (auto k = 0; k < (int)subPoints.size(); k++)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else if (j->nORNT == 2)
						{
							for (int k = (int)subPoints.size() - 1; k >= 0; k--)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else
						{
							OutputDebugString(_T("Unknown ORNT\n"));
						}
					}
				}
			}
			else if (j->nRRNM == 125)
			{
				ST_COMP_CURVE_RECORD* pCompositeCurveRecord = base.GetCompositeCurveRecord(j->nRRID);

				if (pCompositeCurveRecord)
				{
					std::vector<D2D1_POINT_2F> subPoints;
					GetPoints(pCompositeCurveRecord, subPoints);

					if (subPoints.size() > 0)
					{
						if (j->nORNT == 1)
						{
							for (auto k = 0; k < (int)subPoints.size(); k++)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else if (j->nORNT == 2)
						{
							for (int k = (int)subPoints.size() - 1; k >= 0; k--)
							{
								points.push_back(subPoints.at(k));
							}
						}
						else
						{
							OutputDebugString(_T("Unknown ORNT\n"));
						}
					}
				}
			}
			else
			{
				OutputDebugString(_T("Unknown RRNM\n"));
			}
		}
	}
}

#pragma warning(disable:4244)
void S101Dataset::CreateS100DrawingUnit(ID2D1Factory1* pDirect2dFactory, S100PCManager& s100PCManager)
{
	if (!displayList)
		return;

	for (auto i = displayList->m_vecInstruction.begin(); i != displayList->m_vecInstruction.end(); i++)
	{
		if ((*i) == nullptr)
		{
			continue;
		}

		int id = _ttoi((*i)->m_pFeatureReference->m_strReference);
		int geomType = base.GetGeometryTypeFromFeatureRecord(id);

		if (geomType == 0)
		{
			OutputDebugString(_T("There is no feature space type indicated by Drawing Instruction..\n"));
			continue;
		}

		if ((*i)->m_nTypeOfDrawingInstruction == areaInstruction)
		{
			if (geomType != 130)
			{
				OutputDebugString(_T("Area Instruction Type and Feature Type are inconsistent.!\n"));
				continue;
			}

			AreaInstruction* pAreaInstruction = (AreaInstruction*)(*i);

			// Color Fill
			if (pAreaInstruction->patternName.IsEmpty())
			{
				S100DrawingUnitPolygon* pS100DrawingUnitPolygon = new S100DrawingUnitPolygon();
				pS100DrawingUnitPolygon->type = areaInstruction;
			
				pS100DrawingUnitPolygon->color = s100PCManager.s100ColorProfile.GetColor(std::wstring(pAreaInstruction->colorName));
				ST_FEATURE_RECORD* pFeatureRecord = base.GetFeatureRecord(id);
				pS100DrawingUnitPolygon->pGeometry = CreateSurfaceGeometry(pDirect2dFactory, pFeatureRecord);
				HRESULT hr = pS100DrawingUnitPolygon->pGeometry->GetBounds(D2D1::Matrix3x2F::Identity(), &pS100DrawingUnitPolygon->bounds);

				if (!SUCCEEDED(hr))
				{
					OutputDebugString(_T("Failed to get bounds\n"));
				}

				pS100DrawingUnitPolygon->transparency = pAreaInstruction->transparency;

				s100DrawingUnit.push_back(pS100DrawingUnitPolygon);
			}
			// Pattern Fill
			else
			{
				S100DrawingUnitPatternPolygon* pS100DrawingUnitPolygon = new S100DrawingUnitPatternPolygon();
				pS100DrawingUnitPolygon->type = patternAreaInstruction;
				pS100DrawingUnitPolygon->pGeometry = CreateSurfaceGeometry(pDirect2dFactory, base.GetFeatureRecord(id));

				auto i = s100PCManager.areaFillInfo.patternMap.find(pAreaInstruction->patternName);

				if (i != s100PCManager.areaFillInfo.patternMap.end())
				{
					pS100DrawingUnitPolygon->patternName = i->first;
				}

				s100DrawingUnit.push_back(pS100DrawingUnitPolygon);
			}
		}
		else if ((*i)->m_nTypeOfDrawingInstruction == lineInstruction)
		{
			LineInstruction* pLineInstruction = (LineInstruction*)(*i);

			// Line Instruction is could drawing Feature Type including only surface ,curve,composite curve
			if (geomType != 120 &&
				geomType != 125 &&
				geomType != 130)
			{
				CString strErr;
				strErr.Format(_T("The space types of Line Instruction Type and Feature Type are inconsistent! (Feature ID : %d, Feature Type : %d)\n"), id, geomType);
				OutputDebugString(strErr);
				continue;
			}

			S100DrawingUnitPolyLine* pS100DrawingUnitPolyLine = new S100DrawingUnitPolyLine();
			pS100DrawingUnitPolyLine->type = lineInstruction;
			pS100DrawingUnitPolyLine->color = s100PCManager.s100ColorProfile.GetColor(std::wstring(pLineInstruction->colorName));
			pS100DrawingUnitPolyLine->width = (FLOAT)(pLineInstruction->width / PIXEL_MM);
			pS100DrawingUnitPolyLine->hasDash = pLineInstruction->hasDash;

			if (pLineInstruction->lineStyleReference.IsEmpty())
			{
				pS100DrawingUnitPolyLine->pGeometry = CreatePolyLineGeometry(pDirect2dFactory, base.GetFeatureRecord(id));
			}
			else
			{
				GetPoints(base.GetFeatureRecord(id), pS100DrawingUnitPolyLine->points);
				for (
					auto point_iter = pS100DrawingUnitPolyLine->points.begin();
					point_iter != pS100DrawingUnitPolyLine->points.end();
					point_iter++)
				{
					*point_iter = ProjectionAndApplyFactor(*point_iter);
				}
				pS100DrawingUnitPolyLine->lineStyleReference = pLineInstruction->lineStyleReference;
			}

			s100DrawingUnit.push_back(pS100DrawingUnitPolyLine);
		}
		else if ((*i)->m_nTypeOfDrawingInstruction == pointInstruction)
		{
			PointInstruction* pPointInstruction = (PointInstruction*)(*i);
			if (geomType != 110 &&
				geomType != 120 &&
				geomType != 125 &&
				geomType != 130)
			{
				CString strErr;
				strErr.Format(_T("Space type mismatch between Point Instruction Type and Feature Type (%d)!\n"), geomType);
				OutputDebugString(strErr);
				continue;
			}

			S100DrawingUnitPoint* pS100DrawingUnitPoint = new S100DrawingUnitPoint();
			pS100DrawingUnitPoint->type = pointInstruction;
			pS100DrawingUnitPoint->rotation = pPointInstruction->rotation;

			pS100DrawingUnitPoint->point = CreatePoint(base.GetFeatureRecord(id));
			pS100DrawingUnitPoint->point = ProjectionAndApplyFactor(pS100DrawingUnitPoint->point);

			pS100DrawingUnitPoint->symbolName = pPointInstruction->symbolName;
			s100DrawingUnit.push_back(pS100DrawingUnitPoint);
		}
		else if ((*i)->m_nTypeOfDrawingInstruction == textInstruction)
		{
			TextInstruction* pTextInstruction = (TextInstruction*)(*i);
			if (pTextInstruction->pText)
			{
				for (auto j = pTextInstruction->pText->element.begin(); j != pTextInstruction->pText->element.end(); j++)
				{
					if (geomType != 110 &&
						geomType != 120 &&
						geomType != 125 &&
						geomType != 130)
					{
						CString strErr;
						strErr.Format(_T("Space type mismatch between TextPoint Instruction Type and Feature Type (%d)!\n"), geomType);
						OutputDebugString(strErr);
						continue;
					}

					S100DrawingUnitTextPoint* pS100DrawingUnitTextPoint = new S100DrawingUnitTextPoint();
					pS100DrawingUnitTextPoint->type = textInstruction;
					pS100DrawingUnitTextPoint->rotation = ((TextPackage::TextPoint*)(pTextInstruction->pText))->rotation;

					pS100DrawingUnitTextPoint->point = CreatePoint(base.GetFeatureRecord(id));
					pS100DrawingUnitTextPoint->point = ProjectionAndApplyFactor(pS100DrawingUnitTextPoint->point);

					pS100DrawingUnitTextPoint->text = (*j)->text.c_str();

					switch (pTextInstruction->pText->horizontalAlignment)
					{
					case TextPackage::HorizontalAlignment::end:
						pS100DrawingUnitTextPoint->textAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
						break;
					case TextPackage::HorizontalAlignment::h_center:
						pS100DrawingUnitTextPoint->textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
						break;
					}

					switch (pTextInstruction->pText->verticalAlignment)
					{
					case TextPackage::VerticalAlignment::top:
						pS100DrawingUnitTextPoint->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
						break;
					case TextPackage::VerticalAlignment::v_center:
						pS100DrawingUnitTextPoint->paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
						break;
					}

					pS100DrawingUnitTextPoint->verticalOffset = (*j)->verticalOffset / PIXEL_MM;
					pS100DrawingUnitTextPoint->xOffset = ((TextPackage::TextPoint*)(pTextInstruction->pText))->offset->x / PIXEL_MM;
					pS100DrawingUnitTextPoint->yOffset = ((TextPackage::TextPoint*)(pTextInstruction->pText))->offset->y / PIXEL_MM;
					pS100DrawingUnitTextPoint->verticalOffset = (*j)->verticalOffset / PIXEL_MM;

					s100DrawingUnit.push_back(pS100DrawingUnitTextPoint);
				}
			}
		}
		else if ((*i)->m_nTypeOfDrawingInstruction == augmentedPath)
		{
			AugmentedPath* pAugmentedPath = (AugmentedPath*)(*i);
			S100DrawingUnitAugmentedPath* pS100DrawingUnit = new S100DrawingUnitAugmentedPath();
			pS100DrawingUnit->type = augmentedPath;
			pS100DrawingUnit->_geometryPoint = CreatePoint(base.GetFeatureRecord(id));

			if (pAugmentedPath->_path._segment.size() > 0)
			{
				if (typeid(*pAugmentedPath->_path._segment.front()) == typeid(GraphicBasePackage::ArcByRadius))
				{
					GraphicBasePackage::ArcByRadius* pArcByRadius = (GraphicBasePackage::ArcByRadius*)pAugmentedPath->_path._segment.front();
					pS100DrawingUnit->_pGeometry = CreateGeometryFromArcByRadius(pDirect2dFactory, *pArcByRadius);
				}
				else
				{
					if (OUTPUT_DEBUG_STRING)
					{
						OutputDebugString(_T("Invalid PathSegment Type\n"));
					}
				}
			}

			if (pAugmentedPath->pLineStyle)
			{
				if (typeid(*pAugmentedPath->pLineStyle) == typeid(LineStylesPackage::LineStyle))
				{
					LineStylesPackage::LineStyle* pLineStyle = (LineStylesPackage::LineStyle*)pAugmentedPath->pLineStyle;
					if (pLineStyle->dash.size() > 0)
					{
						pS100DrawingUnit->hasDash = true;
					}

					if (pLineStyle->pen)
					{
						pS100DrawingUnit->_color = s100PCManager.s100ColorProfile.GetColor(pLineStyle->pen->color.GetToken());
						pS100DrawingUnit->_width = pLineStyle->pen->width / PIXEL_MM;
					}
				}
			}
			
			s100DrawingUnit.push_back(pS100DrawingUnit);
		}
		else if ((*i)->m_nTypeOfDrawingInstruction == augmentedRay)
		{
			AugmentedRay* pAugmentedRay = (AugmentedRay*)(*i);
			S100DrawingUnitAugmentedRay* pS100DrawingUnit = new S100DrawingUnitAugmentedRay();
			pS100DrawingUnit->type = augmentedRay;
			pS100DrawingUnit->_geometryPoint = CreatePoint(base.GetFeatureRecord(id));

			if (pAugmentedRay->pLineStyle)
			{
				if (typeid(*pAugmentedRay->pLineStyle) == typeid(LineStylesPackage::LineStyle))
				{
					LineStylesPackage::LineStyle* pLineStyle = (LineStylesPackage::LineStyle*)pAugmentedRay->pLineStyle;
					if (pLineStyle->dash.size() > 0)
					{
						pS100DrawingUnit->hasDash = true;
					}

					if (pLineStyle->pen)
					{
						pS100DrawingUnit->_color = s100PCManager.s100ColorProfile.GetColor(pLineStyle->pen->color.GetToken());
						pS100DrawingUnit->_width = pLineStyle->pen->width / PIXEL_MM;
					}
				}
			}
			double deg2rad = M_PI / 180.0;
			pS100DrawingUnit->_direction = pAugmentedRay->dDirection;
			pS100DrawingUnit->_length = pAugmentedRay->dLength / PIXEL_MM;
			s100DrawingUnit.push_back(pS100DrawingUnit);
		}
	}
}

D2D1_POINT_2F S101Dataset::CreatePointFromSENC(int featureID)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	int geometryType = senc.GetGeometryType(featureID);

	switch (geometryType)
	{
	case 110:
		resultPoint = CreatePointFromPointFeatureRecordFromSENC(featureID);
		break;
	case 120:
		resultPoint = CreatePointFromCurveFeatureRecordFromSENC(featureID);
		break;
	case 125:
		resultPoint = CreatePointFromCompositeCurveFeatureRecordFromSENC(featureID);
		break;
	case 130:
		resultPoint = CreatePointFromSurfaceFeatureRecordFromSENC(featureID);
		break;
	default:
		OutputDebugString(_T("Unvalied Geometry Type in CS100TestView::CreatePoint()\n"));
		break;
	}

	return resultPoint;
}

D2D1_POINT_2F S101Dataset::CreatePointFromPointFeatureRecordFromSENC(int featureID)
{
	return senc.GetPoint(featureID);
}

D2D1_POINT_2F S101Dataset::CreatePointFromCurveFeatureRecordFromSENC(int featureID)
{
	D2D1_POINT_2F a = { 1, 1 };
	return a;
}

D2D1_POINT_2F S101Dataset::CreatePointFromCompositeCurveFeatureRecordFromSENC(int featureID)
{
	D2D1_POINT_2F a = { 1, 1 };
	return a;
}

D2D1_POINT_2F S101Dataset::CreatePointFromSurfaceFeatureRecordFromSENC(int featureID)
{
	D2D1_POINT_2F a = { 1, 1 };
	return a;
}

ID2D1Geometry* S101Dataset::CreateSurfaceGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1Geometry* pGeometry = nullptr;
	HRESULT hr = S_OK;// m_pDirect2dFactory->CreatePathGeometry(&pGeometry);

	std::vector<ID2D1PathGeometry*> m_vecRing;

	if (pFeatureRecord)
	{
		ST_SURFACE_RECORD* pSurfaceRecord = base.GetSurfaceRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

		if (pSurfaceRecord)
		{
			for (auto i = pSurfaceRecord->vecRIAS.begin(); i != pSurfaceRecord->vecRIAS.end(); i++)
			{
				for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
				{
					ID2D1PathGeometry* pRingGeometry = nullptr;
					std::vector<D2D1_POINT_2F> points;

					if (j->nRRNM == 120)
					{
						ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(j->nRRID);
						if (pCurveRecord)
						{
							GetPoints(pCurveRecord, points);
						}
					}
					else if (j->nRRNM == 125)
					{
						ST_COMP_CURVE_RECORD* pCompCurveRecord = base.GetCompositeCurveRecord(j->nRRID);
						if (pCompCurveRecord)
						{
							GetPoints(pCompCurveRecord, points);
						}
					}
					else
					{
						OutputDebugString(_T("Invalied RRNM in RIAS\n"));
					}

					if (points.size() >= 3)
					{
						hr = pDirect2dFactory->CreatePathGeometry(&pRingGeometry);

						if (SUCCEEDED(hr))
						{
							ID2D1GeometrySink *pSink = nullptr;

							hr = pRingGeometry->Open(&pSink);

							if (SUCCEEDED(hr))
							{
								pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
								pSink->BeginFigure(
									ProjectionAndApplyFactor(*points.begin()),
									D2D1_FIGURE_BEGIN_FILLED
								);

								for (auto k = points.begin() + 1; k != points.end(); k++)
								{
									pSink->AddLine(ProjectionAndApplyFactor(*k));
								}

								pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

								hr = pSink->Close();

								if (SUCCEEDED(hr))
								{
									m_vecRing.push_back(pRingGeometry);
								}
								else
								{
									OutputDebugString(_T("Faield to close ring sink\n"));
								}
							}
							else
							{
								OutputDebugString(_T("Faield to Open Ring Sink\n"));
							}
						}
						else
						{
							OutputDebugString(_T("Failed to create ring geometry\n"));
						}
					}
					else
					{
						OutputDebugString(_T("Less point count for Create Surface Geometry\n"));
					}
				}
			}
		}
	}

	hr = pDirect2dFactory->CreateGeometryGroup(
		D2D1_FILL_MODE_ALTERNATE,
		(ID2D1Geometry**)&m_vecRing.front(),
		(UINT32)m_vecRing.size(),
		(ID2D1GeometryGroup**)&pGeometry);

	if (!SUCCEEDED(hr))
	{
		OutputDebugString(_T("Failed to create geometry group\n"));
	}

	return pGeometry;
}

void S101Dataset::GetPoints(ST_FEATURE_RECORD *pFeatureRecord, std::vector<D2D1_POINT_2F>& points)
{
	if (pFeatureRecord)
	{
		int type = base.GetGeometryTypeFromFeatureRecord(pFeatureRecord->stFRID.nRCID);

		if (type == 130) // surface
		{
			ST_SURFACE_RECORD* pSurfaceRecord = base.GetSurfaceRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

			if (pSurfaceRecord)
			{
				if (pSurfaceRecord->vecRIAS.size() > 0)
				{
					auto i = pSurfaceRecord->vecRIAS.front();
					for (auto j = i.vecRRNM.begin(); j != i.vecRRNM.end(); j++)
					{
						if (j->nUSAG == 1)
						{
							if (j->nRRNM == 120)
							{
								ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(j->nRRID);
								if (pCurveRecord)
								{
									GetPoints(pCurveRecord, points);
								}
							}
							else if (j->nRRNM == 125)
							{
								ST_COMP_CURVE_RECORD* pCompCurveRecord = base.GetCompositeCurveRecord(j->nRRID);
								if (pCompCurveRecord)
								{
									GetPoints(pCompCurveRecord, points);
								}
							}
							else
							{
								OutputDebugString(_T("Invalied RRNM in RIAS\n"));
							}
						}
					}
				}
			}
		}
		else if (type == 120) // curve
		{
			ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

			if (pCurveRecord)
			{
				GetPoints(pCurveRecord, points);
			}
		}
		else if (type == 125) // comp curve
		{
			ST_COMP_CURVE_RECORD *pCompCurveRecord = base.GetCompositeCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

			if (pCompCurveRecord)
			{
				GetPoints(pCompCurveRecord, points);
			}
		}
		else
		{
			OutputDebugString(_T("Invalied type in GetPoints()\n"));
		}


	}
}

ID2D1Geometry* S101Dataset::CreatePolyLineGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1Geometry* pGeometry = nullptr;

	int type = base.GetGeometryTypeFromFeatureRecord(pFeatureRecord->stFRID.nRCID);

	if (type == 120)
	{
		pGeometry = CreatePolyLineGeometryFromCurveFeature(pDirect2dFactory, pFeatureRecord);

		if (!pGeometry)
		{
			CString strErr;
			strErr.Format(_T("PolyLine geometry is NULL (Feature ID : %d) from curve\n"), pFeatureRecord->stFRID.nRCID);
			OutputDebugString(strErr);
		}
	}
	else if (type == 125)
	{
		pGeometry = CreatePolyLineGeometryFromCompositeCurveFeature(pDirect2dFactory, pFeatureRecord);

		if (!pGeometry)
		{
			CString strErr;
			strErr.Format(_T("PolyLine geometry is NULL (Feature ID : %d) from comp curve\n"), pFeatureRecord->stFRID.nRCID);
			OutputDebugString(strErr);
		}
	}
	else if (type == 130)
	{
		pGeometry = CreateSurfaceGeometry(pDirect2dFactory, pFeatureRecord);

		if (!pGeometry)
		{
			CString strErr;
			strErr.Format(_T("PolyLine geometry is NULL (Feature ID : %d) from surface\n"), pFeatureRecord->stFRID.nRCID);
			OutputDebugString(strErr);
		}
	}
	else
	{
		OutputDebugString(_T("Invalid Feature Geometry in CreatePolyLineGeometry()\n"));
	}

	return pGeometry;
}

ID2D1PathGeometry* S101Dataset::CreatePolyLineGeometryFromCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1PathGeometry* pGeometry = nullptr;

	std::vector<D2D1_POINT_2F> points;

	ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

	if (pCurveRecord)
	{
		GetPoints(pCurveRecord, points);

		if (points.size() >= 2)
		{
			HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

			if (SUCCEEDED(hr))
			{
				ID2D1GeometrySink *pSink = nullptr;

				hr = pGeometry->Open(&pSink);

				if (SUCCEEDED(hr))
				{
					pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
					pSink->BeginFigure(
						ProjectionAndApplyFactor(*points.begin()),
						D2D1_FIGURE_BEGIN_FILLED
					);

					for (auto k = points.begin() + 1; k != points.end(); k++)
					{
						pSink->AddLine(ProjectionAndApplyFactor(*k));
					}

					pSink->EndFigure(D2D1_FIGURE_END_OPEN);

					hr = pSink->Close();
					SafeRelease(&pSink);

					if (SUCCEEDED(hr))
					{
						return pGeometry;
					}
					else
					{
						OutputDebugString(_T("Faield to close ring sink\n"));
					}
				}
				else
				{
					OutputDebugString(_T("Faield to Open Ring Sink\n"));
				}
			}
			else
			{
				OutputDebugString(_T("Failed to create ring geometry\n"));
			}
		}
		else
		{
			OutputDebugString(_T("Less point count for Polyline\n"));
		}
	}
	else
	{
		OutputDebugString(_T("Invalied RRNM in RIAS\n"));
	}

	return pGeometry;
}


ID2D1PathGeometry* S101Dataset::CreatePolyLineGeometryFromCompositeCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1PathGeometry* pGeometry = nullptr;

	std::vector<D2D1_POINT_2F> points;

	ST_COMP_CURVE_RECORD* pCompositeCurveRecord = base.GetCompositeCurveRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

	if (pCompositeCurveRecord)
	{
		GetPoints(pCompositeCurveRecord, points);

		if (points.size() >= 2)
		{
			HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

			if (SUCCEEDED(hr))
			{
				ID2D1GeometrySink *pSink = nullptr;

				hr = pGeometry->Open(&pSink);

				if (SUCCEEDED(hr))
				{
					pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
					pSink->BeginFigure(
						ProjectionAndApplyFactor(*points.begin()),
						D2D1_FIGURE_BEGIN_FILLED
					);

					for (auto k = points.begin() + 1; k != points.end(); k++)
					{
						pSink->AddLine(ProjectionAndApplyFactor(*k));
					}

					pSink->EndFigure(D2D1_FIGURE_END_OPEN);

					hr = pSink->Close();

					if (SUCCEEDED(hr))
					{
						return pGeometry;
					}
					else
					{
						OutputDebugString(_T("Faield to close ring sink\n"));
					}
				}
				else
				{
					OutputDebugString(_T("Faield to Open Ring Sink\n"));
				}
			}
			else
			{
				OutputDebugString(_T("Failed to create ring geometry\n"));
			}
		}
		else
		{
			OutputDebugString(_T("Less point count for Polyline\n"));
		}
	}
	else
	{
		OutputDebugString(_T("Invalied RRNM in RIAS\n"));
	}

	return pGeometry;
}

ID2D1PathGeometry* S101Dataset::CreatePolyLineGeometryFromSurfaceFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord)
{
	ID2D1PathGeometry* pGeometry = nullptr;

	if (pFeatureRecord)
	{
		ST_SURFACE_RECORD* pSurfaceRecord = base.GetSurfaceRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

		if (pSurfaceRecord)
		{
			for (auto i = pSurfaceRecord->vecRIAS.begin(); i != pSurfaceRecord->vecRIAS.end(); i++)
			{
				if (i->vecRRNM.size() >= 1)
				{
					for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
					{
						if (j->nUSAG == 1)
						{
							std::vector<D2D1_POINT_2F> points;

							if (j->nRRNM == 120)
							{
								ST_CURVE_RECORD* pCurveRecord = base.GetCurveRecord(j->nRRID);
								if (pCurveRecord)
								{
									GetPoints(pCurveRecord, points);
								}
							}
							else if (j->nRRNM == 125)
							{
								ST_COMP_CURVE_RECORD* pCompCurveRecord = base.GetCompositeCurveRecord(j->nRRID);
								if (pCompCurveRecord)
								{
									GetPoints(pCompCurveRecord, points);
								}
							}
							else
							{
								OutputDebugString(_T("Invalied RRNM in RIAS\n"));
							}

							if (points.size() >= 3)
							{
								HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);

								if (SUCCEEDED(hr))
								{
									ID2D1GeometrySink *pSink = nullptr;

									hr = pGeometry->Open(&pSink);

									if (SUCCEEDED(hr))
									{
										pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
										pSink->BeginFigure(
											*points.begin(),
											D2D1_FIGURE_BEGIN_HOLLOW
										);

										for (auto k = points.begin() + 1; k != points.end(); k++)
										{
											pSink->AddLine(*k);
										}

										pSink->EndFigure(D2D1_FIGURE_END_OPEN);

										hr = pSink->Close();
										SafeRelease(&pSink);
									}
									else
									{
										OutputDebugString(_T("Faield to Open Ring Sink\n"));
									}
								}
								else
								{
									OutputDebugString(_T("Failed to create ring geometry\n"));
								}
							}
							else
							{
								OutputDebugString(_T("Less point count for Create Surface Geometry\n"));
							}
						}
					}
				}
				else
				{
					OutputDebugString(_T("Less RRNM in create polyline geometry from surface\n"));
				}
			}
		}
	}

	return pGeometry;
}

D2D1_POINT_2F S101Dataset::CreatePoint(ST_FEATURE_RECORD* pFeatureRecord)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	int geometryType = base.GetGeometryTypeFromFeatureRecord(pFeatureRecord->stFRID.nRCID);

	switch (geometryType)
	{
	case 110:
		resultPoint = CreatePointFromPointFeatureRecord(pFeatureRecord);
		break;
	case 120:
		resultPoint = CreatePointFromCurveFeatureRecord(pFeatureRecord);
		break;
	case 125:
		resultPoint = CreatePointFromCompositeCurveFeatureRecord(pFeatureRecord);
		break;
	case 130:
		resultPoint = CreatePointFromSurfaceFeatureRecord(pFeatureRecord);
		break;
	default:
		OutputDebugString(_T("Unvalied Geometry Type in CS100TestView::CreatePoint()\n"));
		break;
	}

	return resultPoint;
}

D2D1_POINT_2F S101Dataset::CreatePointFromPointFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	ST_POINT_RECORD* pPointRecord = base.GetPointRecord(pFeatureRecord->vecSPAS.begin()->vecRRNM.begin()->nRRID);

	if (pPointRecord)
	{
		resultPoint.x = pPointRecord->stC2IT.nXCOO / 10000000.0f;
		resultPoint.y = pPointRecord->stC2IT.nYCOO / 10000000.0f;
	}
	else
	{
		OutputDebugString(_T("Can't get point record in CreatePointFromPointFeatureRecord()\n"));
	}

	return resultPoint;
}

D2D1_POINT_2F S101Dataset::CreatePointFromCurveFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	std::vector<D2D1_POINT_2F> points;

	base.GetPoints(pFeatureRecord, points);

	int _count = (int)points.size();

	if (_count < 1)
	{
		return resultPoint;
	}

	int startIndexOfCenterEdge = (_count - 1) / 2;
	int endIndexOfCenterEdge = startIndexOfCenterEdge++;

	resultPoint.x = (points.at(startIndexOfCenterEdge).x + points.at(endIndexOfCenterEdge).x) / 2;
	resultPoint.y = -((points.at(startIndexOfCenterEdge).y + points.at(endIndexOfCenterEdge).y) / 2);

	return resultPoint;
}

D2D1_POINT_2F S101Dataset::CreatePointFromCompositeCurveFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	std::vector<D2D1_POINT_2F> points;

	base.GetPoints(pFeatureRecord, points);

	int _count = (int)points.size();

	if (_count < 1)
	{
		return resultPoint;
	}

	int startIndexOfCenterEdge = (_count - 1) / 2;
	int endIndexOfCenterEdge = startIndexOfCenterEdge++;

	resultPoint.x = (points.at(startIndexOfCenterEdge).x + points.at(endIndexOfCenterEdge).x) / 2;
	resultPoint.y = -((points.at(startIndexOfCenterEdge).y + points.at(endIndexOfCenterEdge).y) / 2);

	return resultPoint;
}

#pragma warning(disable:4244)
D2D1_POINT_2F S101Dataset::CreatePointFromSurfaceFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord)
{
	D2D1_POINT_2F resultPoint = D2D1::Point2F();

	std::vector<D2D1_POINT_2F> points;

	base.GetPoints(pFeatureRecord, points);

	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	int _cnt = (int)points.size();

	for (int i = 0; i < _cnt; i++)
	{
		j = (i + 1) % _cnt;

		x1 = points.at(i).x;
		y1 = points.at(i).y;
		x2 = points.at(j).x;
		y2 = points.at(j).y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	resultPoint.x = centerX;
	resultPoint.y = -centerY;

	return resultPoint;
}

int S101Dataset::GetUsageFromFileName()
{
	if (fileName.GetLength() < 3) 
		return 0;

	switch (fileName.GetAt(2))
	{
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	}

	return 0;
}

void S101Dataset::DeleteS100DrawingUnit()
{
	for (auto i = s100DrawingUnit.begin(); i != s100DrawingUnit.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}

	s100DrawingUnit.clear();
}

std::vector<S100DrawingUnit*>& S101Dataset::GetS100DrawingUnit()
{
	return s100DrawingUnit;
}

#pragma warning(disable:4244)
ID2D1PathGeometry* S101Dataset::CreateGeometryFromArcByRadius(ID2D1Factory1* pDirect2dFactory, GraphicBasePackage::ArcByRadius& arcByRadius)
{
	if (!arcByRadius._sector)
		return nullptr;

	D2D1_POINT_2F startPoint;
	D2D1_POINT_2F endPoint;
	FLOAT radiusInPixel = arcByRadius._radius / PIXEL_MM;
	startPoint.x = arcByRadius._center.x;
	startPoint.y = arcByRadius._center.y - radiusInPixel;

	double deg2rad = M_PI / 180.0;

	double r1 = arcByRadius._sector->_startAngle * deg2rad;
	double r2 = (arcByRadius._sector->_startAngle + arcByRadius._sector->_angularDistance) * deg2rad;

	startPoint.x = (float)(radiusInPixel * sin(r1));
	startPoint.y = (float)(-radiusInPixel * cos(r1));
	endPoint.x = (float)(radiusInPixel * sin(r2));
	endPoint.y = (float)(-radiusInPixel * cos(r2));

	ID2D1PathGeometry* pGeometry = nullptr;
	HRESULT hr = pDirect2dFactory->CreatePathGeometry(&pGeometry);
	if (!SUCCEEDED(hr) || !pGeometry)
	{
		if (OUTPUT_DEBUG_STRING)
		{
			OutputDebugString(_T("Failed to create geometry in CreateGeometryFromArcByRadius()\n"));
		}
		return nullptr;
	}

	ID2D1GeometrySink* pSink = nullptr;
	hr = pGeometry->Open(&pSink);
	if (!SUCCEEDED(hr) || !pSink)
	{
		if (OUTPUT_DEBUG_STRING)
		{
			OutputDebugString(_T("Failed to open sink in CreateGeometryFromArcByRadius()\n"));
		}
		return nullptr;
	}

	pSink->BeginFigure(
		startPoint,
		D2D1_FIGURE_BEGIN_HOLLOW);

	pSink->AddArc(
		D2D1::ArcSegment(
			endPoint,
			D2D1::SizeF(radiusInPixel, radiusInPixel),
			0,
			D2D1_SWEEP_DIRECTION_CLOCKWISE,
			(arcByRadius._sector->_angularDistance > 180)? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL
		)
	);

	pSink->EndFigure(D2D1_FIGURE_END_OPEN);

	hr = pSink->Close();
	if (!SUCCEEDED(hr))
	{
		if (OUTPUT_DEBUG_STRING)
		{
			OutputDebugString(_T("Failed to close sink in CreateGeometryFromArcByRadius()\n"));
		}
		return nullptr;
	}

	SafeRelease(&pSink);

	return pGeometry;
}