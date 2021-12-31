#include "stdafx.h"
#include "S100Render.h"
#include "EnumTypeOfDrawingInstruction.h"
#include "S100DrawingUnitPoint.h"
#include "S100DrawingUnitPolyLine.h"
#include "S100DrawingUnitPolygon.h"
#include "S100DrawingUnitPatternPolygon.h"
#include "S100DrawingUnitTextPoint.h"
#include "S100DrawingUnitAugmentedPath.h"
#include "S100DrawingUnitAugmentedRay.h"
#include "AreaPatternBitmap.h"


S100Render::S100Render()
{
	InitViewFactor();
}


S100Render::~S100Render()
{

}

void S100Render::DrawBitmapOnLine(
	ID2D1RenderTarget *pRenderTarget,
	D2D1_POINT_2F point1,
	D2D1_POINT_2F point2,
	ID2D1Bitmap *pBitmap,
	D2D1_COLOR_F penColor,
	ID2D1StrokeStyle1 *stroke)
{
	D2D1_SIZE_F bitmapSize = pBitmap->GetSize();

	FLOAT rotation = atan2f(point2.y - point1.y, point2.x - point1.x) * (180 / 3.1415f);

	D2D1::Matrix3x2F oldMatrix;
	pRenderTarget->GetTransform(&oldMatrix);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, point1));

	FLOAT length = sqrtf(pow((point1.x - point2.x), 2.0f) + pow((point2.y - point1.y), 2.0f));

	if (fmodf(length, bitmapSize.width) == 0.0)
	{

	}
	else if (length < 20 && stroke)
	{
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		ID2D1SolidColorBrush *pBrush = nullptr;
		pRenderTarget->CreateSolidColorBrush(penColor, &pBrush);
		pRenderTarget->DrawLine(point1, point2, pBrush, 1, stroke);
		SafeRelease(&pBrush);
		pRenderTarget->SetTransform(oldMatrix);
	}
	else
	{
		D2D1_POINT_2F oldPoint1 = point1;
		FLOAT lastSegmentLength = fmodf(length, bitmapSize.width);

		if (lastSegmentLength == 0.0)
		{
			lastSegmentLength = bitmapSize.width;
		}

		for (int i = 0; i < ceil(length / bitmapSize.width); i++)
		{
			if (i < ceil(length / bitmapSize.width) - 1)
			{
				pRenderTarget->DrawBitmap(
					pBitmap,
					D2D1::RectF(point1.x, point1.y - bitmapSize.height / 2, point1.x + bitmapSize.width, point1.y + bitmapSize.height / 2));
			}
			else
			{
				pRenderTarget->DrawBitmap(
					pBitmap,
					D2D1::RectF(point1.x, point1.y - bitmapSize.height / 2, point1.x + lastSegmentLength, point1.y + bitmapSize.height / 2),
					1,
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					D2D1::RectF(0, 0, lastSegmentLength, bitmapSize.height));
			}

			point1.x += bitmapSize.width;
		}

		pRenderTarget->SetTransform(oldMatrix);
	}
}


void S100Render::DrawBitmapOnPolyline(
	ID2D1RenderTarget *pRenderTarget,
	D2D1_POINT_2F *points,
	int cnt,
	ID2D1Bitmap *pBitmap,
	D2D1_RECT_F viewRect,
	D2D1_COLOR_F penColor,
	ID2D1StrokeStyle1 *stroke)
{
	for (int i = 1; i < cnt; i++)
	{
		D2D1::Matrix3x2F screenMatrix;
		pRenderTarget->GetTransform(&screenMatrix);

		DrawBitmapOnLine(pRenderTarget, points[i - 1], points[i], pBitmap, penColor, stroke);
	}
}


bool S100Render::PointInRect(D2D1_POINT_2F point, D2D1_RECT_F rect)
{
	if (point.x < rect.left || point.x > rect.right)
	{
		return false;
	}
	else if (point.y < rect.top || point.y > rect.bottom)
	{
		return false;
	}
	return true;
}


bool S100Render::OverlapLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect)
{
	if (PointInRect(point1, rect) || PointInRect(point2, rect))
	{
		return true;
	}
	else if (InterSectLineAndRect(point1, point2, rect))
	{
		return true;
	}
	return false;
}


bool S100Render::InterSectLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect)
{
	if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.top, rect.right, rect.top)) // up
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.right, rect.top, rect.right, rect.bottom)) //right
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.bottom, rect.right, rect.bottom)) //down
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.top, rect.left, rect.bottom)) //left
	{
		return true;
	}

	return false;
}


bool S100Render::InterSectLine(
	FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
	FLOAT x3, FLOAT y3, FLOAT x4, FLOAT y4)
{
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0)
	{
		return false;
	}

	// Get the x and y
	float pre = (x1*y2 - y1 * x2), post = (x3*y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	// Check if the x and y coordinates are within both lines
	if (x < min(x1, x2) || x > max(x1, x2) ||
		x < min(x3, x4) || x > max(x3, x4))
	{
		return false;
	}
	if (y < min(y1, y2) || y > max(y1, y2) ||
		y < min(y3, y4) || y > max(y3, y4))
	{
		return false;
	}

	return true;
}


void S100Render::Draw(S100DatasetManager& s100DatasetManager, S100PCManager& s100PCManager, ID2D1Factory1* pDirect2dFactory, ID2D1DCRenderTarget* pRenderTarget, IDWriteTextFormat* pTextFormat, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyleS101Solid, ID2D1StrokeStyle1* pStrokeStyleS101Dash, AreaFillInfo& areaFillInfo, CRect& rectView)
{
	double curZoom = EARTH_2PIR / (FACTOR * PIXEL_MM * scale * 2 * M_PI);
	FLOAT realZoom = (FLOAT)curZoom;

	int width = rectView.right;
	int height = rectView.bottom;

	FLOAT centerX = width * 0.5f;
	FLOAT centerY = height * 0.5f;

	float projectedX = ProjectionX(_x);
	float projectedY = ProjectionY(_y);

	ID2D1RectangleGeometry* pViewRectGeometry = nullptr;
	if (pDirect2dFactory)
	{
		pDirect2dFactory->CreateRectangleGeometry(
			D2D1::RectF((FLOAT)rectView.left, (FLOAT)rectView.top, (FLOAT)rectView.right, (FLOAT)rectView.bottom),
			&pViewRectGeometry);
	}

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// painting the background
	pBrush->SetColor(D2D1::ColorF(1, 1, 1));
	pBrush->SetOpacity(1.0f);
	pRenderTarget->FillRectangle(D2D1::RectF((FLOAT)rectView.left, (FLOAT)rectView.top, (FLOAT)rectView.right, (FLOAT)rectView.bottom), pBrush);

	// Matrix 
	D2D1::Matrix3x2F geom_scale = D2D1::Matrix3x2F::Scale(realZoom, realZoom, D2D1::Point2F(projectedX * FACTOR, -projectedY * FACTOR));
	D2D1::Matrix3x2F geom_trans = D2D1::Matrix3x2F::Translation(-projectedX * FACTOR, projectedY  * FACTOR);
	geomMatrix = geom_scale * geom_trans;

	D2D1::Matrix3x2F screenRot = D2D1::Matrix3x2F::Rotation(_rotation, D2D1::Point2F(0, 0));

	auto screenTransX = centerX;
	auto screenTransY = centerY;
	D2D1::Matrix3x2F screenTrans = D2D1::Matrix3x2F::Translation(screenTransX, screenTransY);
	screenMatrix = screenRot * screenTrans;

	if (screenOffsetX || screenOffsetY)
	{
		D2D1_POINT_2F mp = ProjectionAndApplyFactor(D2D1::Point2F(mapOffsetX, mapOffsetY));
		mp.y = -mp.y;
		D2D1_POINT_2F currentScreenPoint = geomMatrix.TransformPoint(mp);
		currentScreenPoint = screenMatrix.TransformPoint(currentScreenPoint);

		screenMatrix = screenMatrix * D2D1::Matrix3x2F::Translation(
			screenOffsetX - currentScreenPoint.x,
			screenOffsetY - currentScreenPoint.y);

		CString str;
		str.Format(_T("%d, %f\n"), screenOffsetY, currentScreenPoint.y);
		OutputDebugString(str);
	}

	D2D1::Matrix3x2F inverseOfGeometry = geomMatrix;
	D2D1::Matrix3x2F inverseOfScreen = screenMatrix;
	inverseOfGeometry.Invert();
	inverseOfScreen.Invert();

	for (auto j = areaFillInfo.patternMap.begin(); j != areaFillInfo.patternMap.end(); j++)
	{
		j->second->pBitmapBrush->SetTransform(inverseOfScreen);
	}


	D2D1::Matrix3x2F inverseMatrix = inverseOfScreen * inverseOfGeometry;

	FLOAT xmin = 0;
	FLOAT ymin = 0;
	FLOAT xmax = 0;
	FLOAT ymax = 0;
	GetMBRfromRenderTarget(inverseMatrix, rectView, xmin, ymin, xmax, ymax);

	pRenderTarget->SetTransform(geomMatrix * screenMatrix);
	backgroundLayer.Draw(pRenderTarget, pBrush);
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	DrawScale(pRenderTarget, pBrush, pTextFormat);

	DWRITE_TEXT_ALIGNMENT oldTextAlignment = pTextFormat->GetTextAlignment();
	DWRITE_PARAGRAPH_ALIGNMENT oldParagraphAlignment = pTextFormat->GetParagraphAlignment();

	pBrush->SetColor(D2D1::ColorF(0, 0, 0));

	for (int i = 0; i < s100DatasetManager.GetS101DatasetCount(); i++)
	{
		S101Dataset* s101Dataset = s100DatasetManager.Get101Dataset(i);

		if (s101Dataset &&
			IsIntersect(s101Dataset->xmin, s101Dataset->ymin, s101Dataset->xmax, s101Dataset->ymax, xmin, ymin, xmax, ymax) &&
			IsVisible(GetCurrentScale(), s101Dataset))
		{
			OutputDebugString(s101Dataset->GetFilePath() + _T("\n"));
			std::vector<S100DrawingUnit*> drawintUnit = s101Dataset->GetS100DrawingUnit();

			// Area, Line Instruction
			for (auto i = drawintUnit.begin(); i != drawintUnit.end(); i++)
			{
				if ((*i)->type == TypeOfDrawingInstruction::areaInstruction)
				{
					S100DrawingUnitPolygon* pDrawingUnit = (S100DrawingUnitPolygon*)(*i);

					ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
					pDirect2dFactory->CreateTransformedGeometry(pDrawingUnit->pGeometry, geomMatrix, &pTransformedGeometry);

					pBrush->SetColor(pDrawingUnit->color);
					pBrush->SetOpacity(pDrawingUnit->transparency);

					D2D1_GEOMETRY_RELATION relationResult = D2D1_GEOMETRY_RELATION_OVERLAP;
					//pViewRectGeometry->CompareWithGeometry(pTransformedGeometry, screenMatrix, &relationResult);
					if (relationResult == D2D1_GEOMETRY_RELATION_IS_CONTAINED ||
						relationResult == D2D1_GEOMETRY_RELATION_CONTAINS ||
						relationResult == D2D1_GEOMETRY_RELATION_OVERLAP)
					{
						pRenderTarget->SetTransform(screenMatrix);
						pRenderTarget->FillGeometry(pTransformedGeometry, pBrush);
						pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
					}

					SafeRelease(&pTransformedGeometry);
				}
				else if ((*i)->type == TypeOfDrawingInstruction::patternAreaInstruction)
				{
					S100DrawingUnitPatternPolygon* pDrawingUnit = (S100DrawingUnitPatternPolygon*)(*i);

					auto i = areaFillInfo.patternMap.find(pDrawingUnit->patternName);

					if (i != areaFillInfo.patternMap.end())
					{
						ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
						pDirect2dFactory->CreateTransformedGeometry(pDrawingUnit->pGeometry, geomMatrix, &pTransformedGeometry);

						D2D1_GEOMETRY_RELATION relationResult = D2D1_GEOMETRY_RELATION_OVERLAP;
						pViewRectGeometry->CompareWithGeometry(pTransformedGeometry, screenMatrix, &relationResult);
						if (relationResult == D2D1_GEOMETRY_RELATION_IS_CONTAINED ||
							relationResult == D2D1_GEOMETRY_RELATION_CONTAINS ||
							relationResult == D2D1_GEOMETRY_RELATION_OVERLAP)
						{
							pRenderTarget->SetTransform(screenMatrix);
							pRenderTarget->FillGeometry(pTransformedGeometry, i->second->pBitmapBrush);
							pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
						}

						SafeRelease(&pTransformedGeometry);
					}
				}
				else if ((*i)->type == TypeOfDrawingInstruction::lineInstruction)
				{
					S100DrawingUnitPolyLine* pDrawingUnit = (S100DrawingUnitPolyLine*)(*i);
					pBrush->SetColor(pDrawingUnit->color);

					if (pDrawingUnit->pGeometry)
					{
						ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
						pDirect2dFactory->CreateTransformedGeometry(pDrawingUnit->pGeometry, geomMatrix, &pTransformedGeometry);
						D2D1_GEOMETRY_RELATION relationResult = D2D1_GEOMETRY_RELATION_OVERLAP;
						pViewRectGeometry->CompareWithGeometry(pTransformedGeometry, screenMatrix, &relationResult);
						if (relationResult == D2D1_GEOMETRY_RELATION_CONTAINS ||
							relationResult == D2D1_GEOMETRY_RELATION_OVERLAP)
						{
							pRenderTarget->SetTransform(screenMatrix);
							if (pDrawingUnit->hasDash)
							{
								pRenderTarget->DrawGeometry(pTransformedGeometry, pBrush, pDrawingUnit->width, pStrokeStyleS101Dash);
							}
							else
							{
								pRenderTarget->DrawGeometry(pTransformedGeometry, pBrush, pDrawingUnit->width, pStrokeStyleS101Solid);
							}
							pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
						}
					}
					else
					{
						// Converting latitude coordinates to screen coordinates points -> screenpoints
						if (pDrawingUnit->screenPoints.size() == pDrawingUnit->points.size())
						{
							for (int i = 0; i < (int)pDrawingUnit->points.size(); i++)
							{
								pDrawingUnit->screenPoints.at(i) = geomMatrix.TransformPoint(pDrawingUnit->points.at(i));
							}
						}
						else
						{
							pDrawingUnit->screenPoints.clear();
							for (int i = 0; i < (int)pDrawingUnit->points.size(); i++)
							{
								pDrawingUnit->screenPoints.push_back(geomMatrix.TransformPoint(pDrawingUnit->points.at(i)));
							}
						}

						pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
						ID2D1Bitmap *pBitmap = nullptr;
						IWICBitmap *pIWICBitmap = nullptr;

						if (s100PCManager.lineImageMap.Lookup(pDrawingUnit->lineStyleReference + _T(".xml"), pIWICBitmap))
						{
							// Change device-independent bitmap to dependent bitmap.
							HRESULT hr = pRenderTarget->CreateBitmapFromWicBitmap(pIWICBitmap, &pBitmap);

							if (!SUCCEEDED(hr))
							{
								OutputDebugString(_T("Failed to create bitmap\n"));
							}
							else
							{
								// Follow the line and draw a beatmap.
								pRenderTarget->SetTransform(screenMatrix);
								DrawBitmapOnPolyline(
									pRenderTarget,
									&pDrawingUnit->screenPoints.front(),
									(int)pDrawingUnit->screenPoints.size(),
									pBitmap,
									D2D1::RectF((FLOAT)rectView.left, (FLOAT)rectView.top, (FLOAT)rectView.right, (FLOAT)rectView.bottom));
								pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
								SafeRelease(&pBitmap);
							}
						}
						else
						{
							CString strErr;
							strErr.Format(_T("Failed to find image (%s.xml)\n"), pDrawingUnit->lineStyleReference);
							OutputDebugString(strErr);
						}
					}
				}
				else if ((*i)->type == TypeOfDrawingInstruction::pointInstruction)
				{
					S100DrawingUnitPoint* pDrawingUnit = (S100DrawingUnitPoint*)(*i);
					D2D1_POINT_2F symbolPoint = pDrawingUnit->point;
					symbolPoint.y = -symbolPoint.y;
					D2D1_POINT_2F curPoint = geomMatrix.TransformPoint(symbolPoint);
					pRenderTarget->SetTransform(screenMatrix);
					s100PCManager.Draw(std::wstring(pDrawingUnit->symbolName), pRenderTarget, pBrush, pStrokeStyleS101Solid, curPoint, pDrawingUnit->rotation - _rotation);
					pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				}
				else if ((*i)->type == TypeOfDrawingInstruction::textInstruction)
				{
					S100DrawingUnitTextPoint* pDrawingUnit = (S100DrawingUnitTextPoint*)(*i);
					pTextFormat->SetTextAlignment(pDrawingUnit->textAlignment);
					pTextFormat->SetParagraphAlignment(pDrawingUnit->paragraphAlignment);
					D2D1_POINT_2F symbolPoint = pDrawingUnit->point;
					symbolPoint.y = -symbolPoint.y;
					D2D1_POINT_2F curPoint = geomMatrix.TransformPoint(symbolPoint);
					curPoint = screenMatrix.TransformPoint(curPoint);
					pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

					pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
					pRenderTarget->DrawTextW(pDrawingUnit->text, pDrawingUnit->text.GetLength(), pTextFormat, D2D1::RectF(
						curPoint.x - 1 + pDrawingUnit->xOffset,
						curPoint.y - 1 + pDrawingUnit->yOffset + pDrawingUnit->verticalOffset,
						curPoint.x + 1 + pDrawingUnit->xOffset,
						curPoint.y + 1 + pDrawingUnit->yOffset + pDrawingUnit->verticalOffset), pBrush);
				}
				else if ((*i)->type == TypeOfDrawingInstruction::augmentedPath)
				{
					S100DrawingUnitAugmentedPath* pDrawingUnit = (S100DrawingUnitAugmentedPath*)(*i);

					D2D1_POINT_2F geomPoint = pDrawingUnit->_geometryPoint;
					geomPoint = ProjectionAndApplyFactor(geomPoint);
					geomPoint.y = -geomPoint.y;

					D2D1_POINT_2F screenPoint = geomMatrix.TransformPoint(geomPoint);
					screenPoint = screenMatrix.TransformPoint(screenPoint);

					pBrush->SetColor(pDrawingUnit->_color);
					pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(_rotation) * D2D1::Matrix3x2F::Translation(screenPoint.x, screenPoint.y));

					if (pDrawingUnit->_pGeometry)
					{
						if (pDrawingUnit->hasDash)
						{
							pRenderTarget->DrawGeometry(
								pDrawingUnit->_pGeometry,
								pBrush,
								pDrawingUnit->_width,
								pStrokeStyleS101Dash);
						}
						else
						{
							pRenderTarget->DrawGeometry(
								pDrawingUnit->_pGeometry,
								pBrush,
								pDrawingUnit->_width,
								pStrokeStyleS101Solid);
						}
					}
				}
				else if ((*i)->type == TypeOfDrawingInstruction::augmentedRay)
				{
					S100DrawingUnitAugmentedRay* pDrawingUnit = (S100DrawingUnitAugmentedRay*)(*i);

					D2D1_POINT_2F geomPoint = pDrawingUnit->_geometryPoint;
					geomPoint = ProjectionAndApplyFactor(geomPoint);
					geomPoint.y = -geomPoint.y;

					D2D1_POINT_2F endPoint = geomPoint;
					endPoint.y -= pDrawingUnit->_length;

					D2D1_POINT_2F screenPoint = geomMatrix.TransformPoint(geomPoint);
					screenPoint = screenMatrix.TransformPoint(screenPoint);

					pBrush->SetColor(pDrawingUnit->_color);
					pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(_rotation + pDrawingUnit->_direction) * D2D1::Matrix3x2F::Translation(screenPoint.x, screenPoint.y));

					if (pDrawingUnit->hasDash)
					{
						pRenderTarget->DrawLine(
							geomPoint,
							endPoint,
							pBrush,
							pDrawingUnit->_width,
							pStrokeStyleS101Dash);
					}
					else
					{
						pRenderTarget->DrawLine(
							geomPoint,
							endPoint,
							pBrush,
							pDrawingUnit->_width,
							pStrokeStyleS101Dash);
					}
				}
			}
		}
	}



	pTextFormat->SetTextAlignment(oldTextAlignment);
	pTextFormat->SetParagraphAlignment(oldParagraphAlignment);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	DrawScale(pRenderTarget, pBrush, pTextFormat);

	pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	SafeRelease(&pViewRectGeometry);

	screenOffsetX = 0;
	screenOffsetY = 0;
	mapOffsetX = 0;
	mapOffsetY = 0;
}

FLOAT S100Render::GetMaxZoom()
{
	FLOAT maxZoom = (FLOAT)(EARTH_2PIR / (PIXEL_MM * 1000.0f * 360.0f));

	return maxZoom;
}

void S100Render::DrawScale(ID2D1RenderTarget *pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat)
{
	D2D1::Matrix3x2F oldMatrix;
	pRenderTarget->GetTransform(&oldMatrix);
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	int currentScale = (int)GetCurrentScale();

	CString strScale;
	strScale.Format(_T("1 : %d"), currentScale);

	pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	pRenderTarget->DrawTextW(strScale, strScale.GetLength(), pTextFormat, D2D1::RectF(0, 0, 500, 100), pBrush);

	pRenderTarget->SetTransform(oldMatrix);
}

#pragma warning(disable:4244)
FLOAT S100Render::GetCurrentScale()
{
	return scale;
}

void S100Render::CreateBackgroundGeometry(ID2D1Factory1* pD2Factory)
{
	backgroundLayer.CreateGeometry(pD2Factory, &backgroundSHPFile);
}

#pragma warning(disable:4244)
void S100Render::KeyDown(UINT nChar, CRect& rectView, int dpiX, int dpiY)
{
	FLOAT maxZoom = GetMaxZoom();

	int moveFactor = 50;
	FLOAT offset = 10;

	double zoomFactor = 2.0f;
	if (IsCTRLPressed())
	{
		offset *= 0.01f;
		zoomFactor = 1.1f;
	}

	switch (nChar)
	{
	case VK_LEFT:
		Move(-offset, 0);
		break;
	case VK_RIGHT:
		Move(offset, 0);
		break;
	case VK_UP:
		Move(0, offset);
		break;
	case VK_DOWN:
		Move(0, -offset);
		break;
	case VK_PRIOR:
		ZoomIn(zoomFactor);
		break;
	case VK_NEXT:
		ZoomOut(1.0 / zoomFactor);
		break;
	case VK_HOME:
		RotationClockwise();
		break;
	case VK_END:
		RotationAntiClockwise();
		break;
	case VK_F2:
	{
		int screenWidth = (int)(rectView.Width() * (96.0 / dpiX));
		int screenHeight = (int)(rectView.Height() * (96.0 / dpiY));
		double centerX = screenWidth / 2.0;
		double centerY = screenHeight / 2.0;

		D2D1_POINT_2F chartCenterPoint;
		chartCenterPoint.x = (xmin + xmax) / 2.0f;
		chartCenterPoint.y = -(ymin + ymax) / 2.0f;

		_x = chartCenterPoint.x;
		_y = -chartCenterPoint.y;

		float projectedXmin = ProjectionX(xmin) * FACTOR;
		float projectedYmin = ProjectionX(ymin) * FACTOR;
		float projectedXmax = ProjectionX(xmax) * FACTOR;
		float projectedYmax = ProjectionX(ymax) * FACTOR;

		double earthPixel = rectView.Width() * (360.0 / (xmax - xmin));
		scale = (EARTH_2PIR / PIXEL_MM) / earthPixel;
	}
	break;
	}
}

#pragma warning(disable:4244)
void S100Render::GetMBRfromRenderTarget(D2D1::Matrix3x2F matrix, CRect& rect, FLOAT& xmin, FLOAT& ymin, FLOAT& xmax, FLOAT& ymax)
{
	D2D1_POINT_2F screenLeftBottomPoint;
	D2D1_POINT_2F wgs84LeftBottomPoint;
	screenLeftBottomPoint.x = rect.left;
	screenLeftBottomPoint.y = rect.bottom;
	wgs84LeftBottomPoint = matrix.TransformPoint(screenLeftBottomPoint);
	wgs84LeftBottomPoint.x /= FACTOR;
	wgs84LeftBottomPoint.y /= -FACTOR;

	D2D1_POINT_2F screenTopRightPoint;
	D2D1_POINT_2F wgs84TopRightPoint;
	screenTopRightPoint.x = rect.right;
	screenTopRightPoint.y = rect.top;
	wgs84TopRightPoint = matrix.TransformPoint(screenTopRightPoint);
	wgs84TopRightPoint.x /= FACTOR;
	wgs84TopRightPoint.y /= -FACTOR;

	xmin = InverseProjectionX(wgs84LeftBottomPoint.x);
	ymin = InverseProjectionY(wgs84LeftBottomPoint.y);
	xmax = InverseProjectionX(wgs84TopRightPoint.x);
	ymax = InverseProjectionY(wgs84TopRightPoint.y);

	if (xmin > xmax)
	{
		FLOAT temp = xmin;
		xmin = xmax;
		xmax = temp;
	}

	if (ymin > ymax)
	{
		FLOAT temp = ymin;
		ymin = ymax;
		ymax = temp;
	}

	CString str;
	str.Format(_T("%f, %f, %f, %f\n"), xmin, ymin, xmax, ymax);
	OutputDebugString(str);
}


bool S100Render::IsIntersect(FLOAT xmin1, FLOAT ymin1, FLOAT xmax1, FLOAT ymax1, FLOAT xmin2, FLOAT ymin2, FLOAT xmax2, FLOAT ymax2)
{
	if (xmax1 < xmin2 ||
		xmin1 > xmax2 ||
		ymax1 < ymin2 ||
		ymin1 > ymax2)
	{
		return false;
	}

	return true;
}


bool S100Render::IsVisible(FLOAT currentScale, S101Dataset* s101Dataset)
{
	switch (s101Dataset->usage)
	{
	case 1:
		if (currentScale <= 2500000) return true;
		break;
	case 2:
		if (currentScale <= 1500000) return true;
		break;
	case 3:
		if (currentScale <= 500000) return true;
		break;
	case 4:
		if (currentScale <= 150000) return true;
		break;
	case 5:
		if (currentScale <= 70000) return true;
		break;
	case 6:
		if (currentScale <= 25000) return true;
		break;
	}

	return false;
}


#pragma warning(disable:4244)
void S100Render::SetViewFactor(double x, double y, long long zoom)
{
	_x = (x * FACTOR);
	_y = (y * FACTOR);
}


#pragma warning(disable:4244)
void S100Render::ZoomIn(double factor)
{
	scale /= factor;

	if (scale < 1000)
	{
		scale = 1000;
	}
}


#pragma warning(disable:4244)
void S100Render::ZoomOut(double factor)
{
	scale /= factor;

	if (scale > 100000000)
	{
		scale = 100000000;
	}
}


#pragma warning(disable:4244)
void S100Render::Move(double x, double y)
{
	_x += x;
	_y += y;

	if (_x < -180)
	{
		_x = -180;
	}
	else if (_x > 180)
	{
		_x = 180;
	}

	if (_y < -LAT_LIMIT)
	{
		_y = -LAT_LIMIT;
	}
	else if (_y > LAT_LIMIT)
	{
		_y = LAT_LIMIT;
	}
}


void S100Render::RotationClockwise()
{
	_rotation += 30.0f;
}


void S100Render::RotationAntiClockwise()
{
	_rotation -= 30.0f;
}


void S100Render::InitViewFactor()
{
	_x = -150;
	_y = 10;
	_rotation = 0;
	scale = 100000000;
}


void S100Render::SetMapPointAtScreenPoint(FLOAT mx, FLOAT my, int sx, int sy)
{
	mapOffsetX = mx;
	mapOffsetY = my;
	screenOffsetX = sx;
	screenOffsetY = sy;
}


#pragma warning(disable:4244)
void S100Render::GetMapPointByScreenPoint(int sx, int sy, FLOAT* mx, FLOAT* my)
{
	D2D1::Matrix3x2F inverseOfGeometry = geomMatrix;
	D2D1::Matrix3x2F inverseOfScreen = screenMatrix;
	inverseOfGeometry.Invert();
	inverseOfScreen.Invert();

	D2D1::Matrix3x2F inverseMatrix = inverseOfScreen * inverseOfGeometry;
	D2D1_POINT_2F mapPoint = inverseMatrix.TransformPoint(D2D1::Point2F(sx, sy));

	mapPoint.x /= FACTOR;
	mapPoint.y /= FACTOR;

	*mx = InverseProjectionX(mapPoint.x);
	*my = -InverseProjectionY(mapPoint.y);
}


void S100Render::SetOffset(int x, int y)
{
	screenOffsetX = x;
	screenOffsetY = y;
}


#pragma warning(disable:4244)
void S100Render::SetCenterPointByScreen(int x, int y)
{
	D2D1::Matrix3x2F inverseOfGeometry = geomMatrix;
	D2D1::Matrix3x2F inverseOfScreen = screenMatrix;
	inverseOfGeometry.Invert();
	inverseOfScreen.Invert();

	D2D1::Matrix3x2F inverseMatrix = inverseOfScreen * inverseOfGeometry;
	D2D1_POINT_2F mapPoint = inverseMatrix.TransformPoint(D2D1::Point2F(x, y));

	mapPoint.x /= FACTOR;
	mapPoint.y /= FACTOR;

	_x = InverseProjectionX(mapPoint.x);
	_y = -InverseProjectionY(mapPoint.y);

	if (_x < -180)
	{
		_x = -180;
	}
	else if (_x > 180)
	{
		_x = 180;
	}

	if (_y < -LAT_LIMIT)
	{
		_y = -LAT_LIMIT;
	}
	else if (_y > LAT_LIMIT)
	{
		_y = LAT_LIMIT;
	}
}