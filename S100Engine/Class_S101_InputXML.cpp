#include "stdafx.h"
#include "Class_S101_InputXML.h"
#include "Class_S101_Base.h"

#include <algorithm>

#define CHECK_AND_RELEASE(pInterface)  \
if (pInterface) \
{\
pInterface->Release(); \
pInterface = NULL; \
}\

#define RELEASE(pInterface)  \
{\
pInterface->Release(); \
pInterface = NULL; \
}\


CClass_S101_InputXML::CClass_S101_InputXML()
{
	hr_Initialize = ::CoInitialize(NULL);

	if (!SUCCEEDED(hr_Initialize))
	{
		OutputDebugString(_T("Failed to call CoInitialize()\n"));
	}
}


CClass_S101_InputXML::~CClass_S101_InputXML()
{
	if (SUCCEEDED(hr_Initialize))
	{
		::CoUninitialize();
	}
}


MSXML2::IXMLDOMDocument2Ptr CClass_S101_InputXML::GetInputXML(CClass_S101_Base* pBase)
{
	MSXML2::IXMLDOMDocument2Ptr pDoc = NULL;
	pDoc.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));
	if (!pDoc)
	{
		OutputDebugString(_T("Failed to call MSXML2::IXMLDOMDocument2Ptr::CreateInstance()\n"));
		return nullptr; //Failed to create DOMDocument Instance.
	}

	CreateProcessingInstruction(pDoc);
	MSXML2::IXMLDOMElementPtr pRootElement = CreateDatasetElement(pDoc);
	MSXML2::IXMLDOMElementPtr pFeaturesElement = CreateFeaturesElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pInformationTypesElement = CreateInformationTypesElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pPointsElement = CreatePointsElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pMultiPointsElement = CreateMultiPointsElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pCurvesElement = CreateCurvesElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pCompositeCurvesElement = CreateCompositeCurvesElement(pBase, pDoc, pRootElement);
	MSXML2::IXMLDOMElementPtr pSurfacesElement = CreateSurfacesElement(pBase, pDoc, pRootElement);

	return pDoc;
}


bool CClass_S101_InputXML::GetInputXML(CClass_S101_Base* pBase, CString pathInputXML)
{
	MSXML2::IXMLDOMDocument2Ptr pInputXML = GetInputXML(pBase);
	if (pInputXML)
	{
		HRESULT hr = pInputXML->save(_variant_t(pathInputXML));

		if (SUCCEEDED(hr))
		{
			return true;
		}
	}

	return false;
}


MSXML2::IXMLDOMDocument2Ptr CClass_S101_InputXML::GetOutputXML(CString pathInputXML, CString pathXSL)
{
	MSXML2::IXMLDOMDocument2Ptr pInputXML = NULL;
	pInputXML.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));
	if (pInputXML)
	{
		OutputDebugString(_T("Failed to call MSXML2::IXMLDOMDocument2Ptr::CreateInstance()\n"));
		return NULL; // Failed to create DOMDocument Instance.
	}

	if (pInputXML->load(_variant_t(pathInputXML)))
	{
		return GetOutputXML(pInputXML, pathXSL);
	}

	return NULL;
}


bool CClass_S101_InputXML::GetOutputXML(CString pathInputXML, CString pathXSL, CString pathOutputXML)
{
	MSXML2::IXMLDOMDocument2Ptr pOutputXML = GetOutputXML(pathInputXML, pathXSL);

	if (pOutputXML)
	{
		if (SUCCEEDED(pOutputXML->save(_variant_t(pathOutputXML))))
		{
			return true;
		}
	}

	return false;
}


bool CClass_S101_InputXML::GetOutputXML(MSXML2::IXMLDOMDocument2Ptr pInputXML, CString pathXSL, CString pathOutputXML)
{
	MSXML2::IXMLDOMDocument2Ptr pOutputXML = GetOutputXML(pInputXML, pathXSL);

	if (pOutputXML)
	{
		if (SUCCEEDED(pOutputXML->save(_variant_t(pathOutputXML))))
		{
			return true;
		}
	}

	return false;
}


MSXML2::IXMLDOMDocument2Ptr CClass_S101_InputXML::GetOutputXML(MSXML2::IXMLDOMDocument2Ptr pInputXML, CString pathXSL)
{
	HRESULT hr;
	MSXML2::IXMLDOMDocument2Ptr pStyleSheet = NULL;
	MSXML2::IXMLDOMDocument2Ptr pOutputXML = NULL;
	MSXML2::IXSLTemplatePtr pIXSLTemplate = NULL;
	MSXML2::IXSLProcessorPtr pIXSLProcessor = NULL;

	try
	{
		pIXSLTemplate.CreateInstance(__uuidof(MSXML2::XSLTemplate));

		if (pIXSLTemplate)
		{
			pStyleSheet.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));

			if (pStyleSheet)
			{
				hr = pStyleSheet->put_async(VARIANT_FALSE);

				if (SUCCEEDED(hr))
				{
					if (pStyleSheet->load(_variant_t(pathXSL)))
					{
						pIXSLTemplate->stylesheet = pStyleSheet.GetInterfacePtr();

						if (SUCCEEDED(hr))
						{
							pIXSLProcessor = pIXSLTemplate->createProcessor();

							if (pIXSLProcessor)
							{
								pInputXML->async = VARIANT_FALSE;

								if (SUCCEEDED(hr))
								{
									pIXSLProcessor->input = variant_t((IUnknown*)pInputXML);

									if (SUCCEEDED(hr))
									{
										pOutputXML.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument));

										if (pOutputXML)
										{
											pOutputXML->async = VARIANT_FALSE;

											if (SUCCEEDED(hr))
											{
												pIXSLProcessor->output = variant_t((IUnknown*)pOutputXML);

												if (SUCCEEDED(hr))
												{
													if (AddParameters(pIXSLProcessor))
													{
														if (pIXSLProcessor->transform())
														{
															return pOutputXML;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	catch (_com_error &e)
	{
		CHECK_AND_RELEASE(pIXSLTemplate);
		CHECK_AND_RELEASE(pStyleSheet);
		CHECK_AND_RELEASE(pOutputXML);
		OutputDebugString(e.ErrorMessage());
		return NULL;
	}

	return NULL;
}

CString CClass_S101_InputXML::GetInformationAssociationStringCode(CClass_S101_Base* pBase, int nNumeric)
{
	for (auto i = pBase->m_stDSGI.vecIACS.begin(); i != pBase->m_stDSGI.vecIACS.end(); i++)
	{
		if (i->nNumeric == (unsigned int)nNumeric)
		{
			return i->strCode;
		}
	}

	return _T("");
}


CString CClass_S101_InputXML::GetFeatureAssociationStringCode(CClass_S101_Base* pBase, int nNumeric)
{
	for (auto i = pBase->m_stDSGI.vecFACS.begin(); i != pBase->m_stDSGI.vecFACS.end(); i++)
	{
		if (i->nNumeric == (unsigned int)nNumeric)
		{
			return i->strCode;
		}
	}

	return _T("");
}


CString CClass_S101_InputXML::GetAssociationRoleStringCode(CClass_S101_Base* pBase, int nNumeric)
{
	for (auto i = pBase->m_stDSGI.vecARCS.begin(); i != pBase->m_stDSGI.vecARCS.end(); i++)
	{
		if (i->nNumeric == (unsigned int)nNumeric)
		{
			return i->strCode;
		}
	}

	return _T("");
}


bool CClass_S101_InputXML::CreateProcessingInstruction(MSXML2::IXMLDOMDocument2Ptr pDoc)
{
	MSXML2::IXMLDOMProcessingInstructionPtr pProcessingInstruction = pDoc->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding=\"UTF-8\"");
	pDoc->appendChild(pProcessingInstruction);
	return true;
}


MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateDatasetElement(MSXML2::IXMLDOMDocument2Ptr pDoc)
{
	MSXML2::IXMLDOMElementPtr pRootElement = pDoc->createElement(L"Dataset");
	pRootElement->setAttribute((_bstr_t)_T("xmlns:xsi"), (_variant_t)_T("http://www.w3.org/2001/XMLSchema-instance"));
	pRootElement->setAttribute((_bstr_t)_T("xmlns:noNamespaceSchemaLocation"), (_variant_t)_T("S101DataModel.xsd"));
	pRootElement->setAttribute((_bstr_t)_T("xmlns:S100"), (_variant_t)_T("http://www.iho.int/S100BaseModel"));
	pDoc->appendChild(pRootElement);
	return pRootElement;
}


MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateFeaturesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	MSXML2::IXMLDOMElementPtr pFeaturesElement = pDoc->createElement(L"Features");
	pDatasetElement->appendChild(pFeaturesElement);

	CreateFeatureTypeStringCodeElement(pBase, pDoc, pFeaturesElement);

	return pFeaturesElement;
}


MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateInformationTypesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	MSXML2::IXMLDOMElementPtr pInformationTypesElement = pDoc->createElement(L"InformationTypes");
	pDatasetElement->appendChild(pInformationTypesElement);

	CreateInformationTypeStringCodeElement(pBase, pDoc, pInformationTypesElement);

	return pInformationTypesElement;
}

MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreatePointsElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	MSXML2::IXMLDOMElementPtr pPointsElement = pDoc->createElement(L"Points");
	pDatasetElement->appendChild(pPointsElement);

	CreatePointElement(pBase, pDoc, pPointsElement);

	return pPointsElement;
}

MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateMultiPointsElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	MSXML2::IXMLDOMElementPtr pMultiPointsElement = pDoc->createElement(L"MultiPoints");
	pDatasetElement->appendChild(pMultiPointsElement);

	CreateMultiPointElement(pBase, pDoc, pMultiPointsElement);

	return pMultiPointsElement;
}

MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateCurvesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	MSXML2::IXMLDOMElementPtr pCurvesElement = pDoc->createElement(L"Curves");
	pDatasetElement->appendChild(pCurvesElement);
	
	CreateCurveElement(pBase, pDoc, pCurvesElement);

	return pCurvesElement;
}

MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateCompositeCurvesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	CreateCompositeCurveElement(pBase, pDoc, pDatasetElement);
	return pDatasetElement;
}

MSXML2::IXMLDOMElementPtr CClass_S101_InputXML::CreateSurfacesElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pDatasetElement)
{
	CreateSurfaceElement(pBase, pDoc, pDatasetElement);
	return pDatasetElement;
}

bool CClass_S101_InputXML::CreateFeatureTypeStringCodeElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pFeaturesElement)
{
	for (auto i = pBase->m_vecFeatureRecord.begin(); i != pBase->m_vecFeatureRecord.end(); i++)
	{
		CString strFeatureStringCode = pBase->GetFeatureTypeCamelCase(i->stFRID.nNFTC);
		CString strID = GetFeatureTypeID(&*i);
		CString strPrimitive = GetFeatureTypePrimitive(&*i);
		
		// Create Feature Type Element
		MSXML2::IXMLDOMElementPtr pFeatureTypeStringCodeElement = pDoc->createElement((_bstr_t)strFeatureStringCode);
		pFeatureTypeStringCodeElement->setAttribute((_bstr_t)_T("id"), (_variant_t)strID);
		pFeatureTypeStringCodeElement->setAttribute((_bstr_t)_T("primitive"), (_variant_t)strPrimitive);

		pFeaturesElement->appendChild(pFeatureTypeStringCodeElement);

		// Create Attribute Element
		CreateAttributeStringCodeElement(pBase, &*i, pDoc, pFeatureTypeStringCodeElement);
	}

	return true;
}

bool CClass_S101_InputXML::CreateInformationTypeStringCodeElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pInformationTypesElement)
{
	for (auto i = pBase->m_vecInfoRecord.begin(); i != pBase->m_vecInfoRecord.end(); i++)
	{
		CString strInformationTypeStringCode = pBase->GetInformationTypeCamelCase(i->stIRID.nNITC);
		CString strID = GetInformationTypeID(&*i);

		MSXML2::IXMLDOMElementPtr pInformationTypeStringCodeElement = pDoc->createElement((_bstr_t)strInformationTypeStringCode);
		pInformationTypeStringCodeElement->setAttribute((_bstr_t)_T("id"), (_variant_t)strID);

		pInformationTypesElement->appendChild(pInformationTypeStringCodeElement);

		CreateAttributeStringCodeElement(pBase, &*i, pDoc, pInformationTypeStringCodeElement);
	}

	return true;
}

bool CClass_S101_InputXML::CreatePointElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pPointsElement)
{
	for (auto i = pBase->m_vecPointRecord.begin(); i != pBase->m_vecPointRecord.end(); i++)
	{
		CString strID = GetPointID(&*i);

		MSXML2::IXMLDOMElementPtr pPointElement = pDoc->createElement(L"Point");
		pPointElement->setAttribute(L"id", (_variant_t)strID);

		pPointsElement->appendChild(pPointElement);

		double x = i->stC2IT.nXCOO / 10000000.0;
		double y = i->stC2IT.nYCOO / 10000000.0;

		CString strX, strY;
		strX.Format(_T("%.7lf"), x);
		strY.Format(_T("%.7lf"), y);

		MSXML2::IXMLDOMElementPtr pCoordinate2DElement = pDoc->createElement(L"Coordinate2D");
		MSXML2::IXMLDOMElementPtr pXElement = pDoc->createElement(L"X");
		MSXML2::IXMLDOMElementPtr pYElement = pDoc->createElement(L"Y");

		pXElement->Puttext((_bstr_t)strX);
		pYElement->Puttext((_bstr_t)strY);

		pCoordinate2DElement->appendChild(pXElement);
		pCoordinate2DElement->appendChild(pYElement);

		pPointElement->appendChild(pCoordinate2DElement);
	}

	return true;
}

bool CClass_S101_InputXML::CreateMultiPointElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pMultiPointsElement)
{
	for (auto i = pBase->m_vecMultiPointRecord.begin(); i != pBase->m_vecMultiPointRecord.end(); i++)
	{
		CString strID = GetMultiPointID(&*i);

		MSXML2::IXMLDOMElementPtr pMultiPointElement = pDoc->createElement(L"MultiPoint");
		pMultiPointElement->setAttribute(L"id", (_variant_t)strID);

		pMultiPointsElement->appendChild(pMultiPointElement);

		for (auto j = i->vecC3IL.begin(); j != i->vecC3IL.end(); j++)
		{
			for (auto k = j->vecXYZ.begin(); k != j->vecXYZ.end(); k++)
			{
				MSXML2::IXMLDOMElementPtr pCoordinate3DElement = pDoc->createElement(L"Coordinate3D");
				MSXML2::IXMLDOMElementPtr pXElement = pDoc->createElement(L"X");
				MSXML2::IXMLDOMElementPtr pYElement = pDoc->createElement(L"Y");
				MSXML2::IXMLDOMElementPtr pZElement = pDoc->createElement(L"Z");

				double x = k->nXCOO / 10000000.0;
				double y = k->nYCOO / 10000000.0;
				double z = k->nZCOO / 100.0;

				CString strX, strY, strZ;
				strX.Format(_T("%.7lf"), x);
				strY.Format(_T("%.7lf"), y);
				strZ.Format(_T("%.2lf"), z);

				pXElement->Puttext((_bstr_t)strX);
				pYElement->Puttext((_bstr_t)strY);
				pZElement->Puttext((_bstr_t)strZ);

				pCoordinate3DElement->appendChild(pXElement);
				pCoordinate3DElement->appendChild(pYElement);
				pCoordinate3DElement->appendChild(pZElement);

				pMultiPointElement->appendChild(pCoordinate3DElement);
			}
		}
	}

	return true;
}

bool CClass_S101_InputXML::CreateCurveElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pCurvesElement)
{
	for (auto vecCurve = pBase->m_vecCurveRecord.begin(); vecCurve != pBase->m_vecCurveRecord.end(); vecCurve++)
	{
		MSXML2::IXMLDOMElementPtr pCurveElement = pDoc->createElement(L"Curve");
		pCurveElement->setAttribute(L"id", (_variant_t)vecCurve->stCRID.nRCID);

		for (auto vecPTAS = vecCurve->vecPTAS.begin(); vecPTAS != vecCurve->vecPTAS.end(); vecPTAS++)
		{
			MSXML2::IXMLDOMElementPtr pBoundaryElement = pDoc->createElement(L"Boundary");
			pBoundaryElement->setAttribute(L"ref", (_variant_t)vecPTAS->nRRID);


			switch (vecPTAS->nTOPI)
			{
			case 1:
				pBoundaryElement->setAttribute(L"boundaryType", (_variant_t)"Begin");
				break;
			case 2:
				pBoundaryElement->setAttribute(L"boundaryType", (_variant_t)"End");
				break;
			case 3:
				pBoundaryElement->setAttribute(L"boundaryType", (_variant_t)"End");

				MSXML2::IXMLDOMElementPtr pBoundaryElement2 = pDoc->createElement(L"Boundary");
				pBoundaryElement2->setAttribute(L"ref", (_variant_t)vecPTAS->nRRID);
				pBoundaryElement2->setAttribute(L"boundaryType", (_variant_t)"Begin");
				pCurveElement->appendChild(pBoundaryElement2);
				break;
			}
			pCurveElement->appendChild(pBoundaryElement);
		}
		for (auto vecSEGH = vecCurve->vecSEGH.begin(); vecSEGH != vecCurve->vecSEGH.end(); vecSEGH++)
		{
			MSXML2::IXMLDOMElementPtr pSegmentElement = pDoc->createElement(L"Segment");
			if (vecSEGH == vecCurve->vecSEGH.begin())
			{
				pSegmentElement->setAttribute(L"interpolation", (_variant_t)GetInterpolation(vecSEGH->nINTP));
			}
			else
			{
				OutputDebugStringW(L"There is at least one value for segment.");
			}
			for (auto vecC2IL = vecCurve->vecC2IL.begin(); vecC2IL != vecCurve->vecC2IL.end(); vecC2IL++)
			{
				MSXML2::IXMLDOMElementPtr pControlPointElement = pDoc->createElement(L"ControlPoint");

				CString x, y;
				x.Format(_T("%d"), (vecC2IL->nXCOO / 10000000));
				y.Format(_T("%d"), (vecC2IL->nYCOO / 10000000));
				MSXML2::IXMLDOMElementPtr pXElement = pDoc->createElement(L"x");
				pXElement->text = (_bstr_t)x;
				pControlPointElement->appendChild(pXElement);

				MSXML2::IXMLDOMElementPtr pYElement = pDoc->createElement(L"y");
				pYElement->text = (_bstr_t)y;
				pControlPointElement->appendChild(pYElement);

				pSegmentElement->appendChild(pControlPointElement);
			}


			pCurveElement->appendChild(pSegmentElement);
		}

		pCurvesElement->appendChild(pCurveElement);
	}

	return true;
}

bool CClass_S101_InputXML::CreateCompositeCurveElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pCompositeCurveElement)
{
	MSXML2::IXMLDOMElementPtr pCompositeCurvesElement = pDoc->createElement(L"CompositeCurves");
	for (auto vecCompCurve = pBase->m_vecCompCurveRecord.begin(); vecCompCurve != pBase->m_vecCompCurveRecord.end(); vecCompCurve++)
	{
		for (auto vecCUCO = vecCompCurve->vecCUCO.begin(); vecCUCO != vecCompCurve->vecCUCO.end(); vecCUCO++)
		{
			MSXML2::IXMLDOMElementPtr pCompositeCurveElement = pDoc->createElement(L"CompositeCurve");
			pCompositeCurveElement->setAttribute(L"id", (_variant_t)vecCompCurve->stCCID.nRCID);

			for (auto vecRRNM = vecCUCO->vecRRNM.begin(); vecRRNM != vecCUCO->vecRRNM.end(); vecRRNM++)
			{
				MSXML2::IXMLDOMElementPtr pCurveORCompositeCurveElement;

				switch (vecRRNM->nRRNM)
				{
				case 120:
					pCurveORCompositeCurveElement = pDoc->createElement(L"Curve");
					pCurveORCompositeCurveElement->setAttribute(L"ref", (_variant_t)vecRRNM->nRRID);
					pCurveORCompositeCurveElement->setAttribute(L"orientation", vecRRNM->nORNT == 1 ? "Forward" : (vecRRNM->nORNT == 2 ? "Reverse" : ""));
					if (vecRRNM->nORNT > 2)
						OutputDebugStringW(L"Invalid value for 'CompositeCurves->CompositeCurve->CurveORCompositeCurve:orientation'");
					break;
				case 125:
					pCurveORCompositeCurveElement = pDoc->createElement(L"CompositeCurve");
					pCurveORCompositeCurveElement->setAttribute(L"ref", (_variant_t)vecRRNM->nRRID);
					pCurveORCompositeCurveElement->setAttribute(L"orientation", vecRRNM->nORNT == 1 ? "Forward" : (vecRRNM->nORNT == 2 ? "Reverse" : ""));
					if (vecRRNM->nORNT > 2)
						OutputDebugStringW(L"Invalid value for 'CompositeCurves->CompositeCurve->CurveORCompositeCurve:orientation'");
					break;
				default:
					pCurveORCompositeCurveElement = nullptr;
					break;
				}
				pCompositeCurveElement->appendChild(pCurveORCompositeCurveElement);
			}

			pCompositeCurvesElement->appendChild(pCompositeCurveElement);
		}
		pCompositeCurveElement->appendChild(pCompositeCurvesElement);
	}
	return true;
}

bool CClass_S101_InputXML::CreateSurfaceElement(CClass_S101_Base* pBase, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pSurfaceElement)
{
	MSXML2::IXMLDOMElementPtr pSurfacesElement = pDoc->createElement(L"Surfaces");
	for (auto vecSurface = pBase->m_vecSurfaceRecord.begin(); vecSurface != pBase->m_vecSurfaceRecord.end(); vecSurface++)
	{
		for (auto vecRIAS = vecSurface->vecRIAS.begin(); vecRIAS != vecSurface->vecRIAS.end(); vecRIAS++)
		{
			MSXML2::IXMLDOMElementPtr pSurfaceElement = pDoc->createElement(L"Surface");
			pSurfaceElement->setAttribute(L"id", (_variant_t)vecSurface->stSRID.nRCID);
			for (auto vecRRNM = vecRIAS->vecRRNM.begin(); vecRRNM != vecRIAS->vecRRNM.end(); vecRRNM++)
			{
				MSXML2::IXMLDOMElementPtr pRingElement = pDoc->createElement(L"Ring");
				pRingElement->setAttribute(L"type", vecRRNM->nUSAG == 1 ? "Outer" : (vecRRNM->nUSAG == 2 ? "Inner" : ""));
				if (vecRRNM->nUSAG > 2)
					OutputDebugStringW(L"Invalid value for 'Surfaces->Surface->Ring:type'");
				
				MSXML2::IXMLDOMElementPtr pCurveORCompositeCurveElement;
				switch (vecRRNM->nRRNM)
				{
				case 120:
					pCurveORCompositeCurveElement = pDoc->createElement(L"Curve");
					pCurveORCompositeCurveElement->setAttribute(L"ref", (_variant_t)vecRRNM->nRRID);
					pCurveORCompositeCurveElement->setAttribute(L"orientation", vecRRNM->nORNT == 1 ? "Forward" : (vecRRNM->nORNT == 2 ? "Reverse" : ""));
					if (vecRRNM->nORNT > 2)
						OutputDebugStringW(L"Invalid value for 'Surfaces->Surface->Ring->CurveORCompositeCurve:orientation'");
					break;
				case 125:
					pCurveORCompositeCurveElement = pDoc->createElement(L"CompositeCurve");
					pCurveORCompositeCurveElement->setAttribute(L"ref", (_variant_t)vecRRNM->nRRID);
					pCurveORCompositeCurveElement->setAttribute(L"orientation", vecRRNM->nORNT == 1 ? "Forward" : (vecRRNM->nORNT == 2 ? "Reverse" : ""));
					if (vecRRNM->nORNT > 2)
						OutputDebugStringW(L"Invalid value for 'Surfaces->Surface->Ring->CurveORCompositeCurve:orientation'");
					break;
				default:
					pCurveORCompositeCurveElement = nullptr;
					OutputDebugStringW(L"CurveORCompositeCurves has an invalid value.");
					break;
				}
				pRingElement->appendChild(pCurveORCompositeCurveElement);

				pSurfaceElement->appendChild(pRingElement);
			}
			pSurfacesElement->appendChild(pSurfaceElement);
		}

		pSurfaceElement->appendChild(pSurfacesElement);
	}
	return true;
}

bool CClass_S101_InputXML::CreateAttributeStringCodeElement(CClass_S101_Base* pBase, ST_FEATURE_RECORD* pST_FEATURE_RECORD, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pFeatureTypeElement)
{
	for (auto i = pST_FEATURE_RECORD->vecSPAS.begin(); i != pST_FEATURE_RECORD->vecSPAS.end(); i++)
	{
		for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
		{
			MSXML2::IXMLDOMElementPtr pSpatialRelationElement = pDoc->createElement((_bstr_t)GetSpatialTypeNameFromRRNM(j->nRRNM));
			pSpatialRelationElement->setAttribute((_bstr_t)_T("id"), (_variant_t)j->nRRID);

			if (!GetSpatialTypeNameFromRRNM(j->nRRNM).Compare(_T("Curve")) ||
				!GetSpatialTypeNameFromRRNM(j->nRRNM).Compare(_T("CompositeCurve")))
			{
				pSpatialRelationElement->setAttribute((_bstr_t)_T("orientation"), (_variant_t)GetOrientationFromORNT(j->nORNT));
			}

			pFeatureTypeElement->appendChild(pSpatialRelationElement);
		}
	}

	for (auto i = pST_FEATURE_RECORD->vecATTR.begin(); i != pST_FEATURE_RECORD->vecATTR.end(); i++)
	{
		std::vector<MSXML2::IXMLDOMElementPtr> vecAttributeStringCodeElement;

		for (auto j = i->vecNATC.begin(); j != i->vecNATC.end(); j++)
		{
			MSXML2::IXMLDOMElementPtr pAttributeStringCodeElement = pDoc->createElement((_bstr_t)pBase->GetAttributeCamelCase(j->nNATC));
			vecAttributeStringCodeElement.push_back(pAttributeStringCodeElement);

			if (j->nPAIX == 0)
			{
				pFeatureTypeElement->appendChild(pAttributeStringCodeElement);
				pAttributeStringCodeElement->Puttext((_bstr_t)j->strATVL);
			}
			else
			{
				try
				{
					vecAttributeStringCodeElement.at(j->nPAIX - 1)->appendChild(pAttributeStringCodeElement);
					pAttributeStringCodeElement->Puttext((_bstr_t)j->strATVL);
				}
				catch (std::out_of_range)
				{
					OutputDebugString(_T("PAIX Index overflow\n"));
				}
			}
		}
	}

	return true;
}

bool CClass_S101_InputXML::CreateAttributeStringCodeElement(CClass_S101_Base* pBase, ST_INFO_RECORD* pST_INFO_RECORD, MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMElementPtr pInformationTypeElement)
{
	for (auto i = pST_INFO_RECORD->vecATTR.begin(); i != pST_INFO_RECORD->vecATTR.end(); i++)
	{
		std::vector<MSXML2::IXMLDOMElementPtr> vecAttributeStringCodeElement;

		for (auto j = i->vecNATC.begin(); j != i->vecNATC.end(); j++)
		{
			MSXML2::IXMLDOMElementPtr pAttributeStringCodeElement = pDoc->createElement((_bstr_t)pBase->GetAttributeCamelCase(j->nNATC));
			vecAttributeStringCodeElement.push_back(pAttributeStringCodeElement);

			if (j->nPAIX == 0)
			{
				pInformationTypeElement->appendChild(pAttributeStringCodeElement);
				pAttributeStringCodeElement->Puttext((_bstr_t)j->strATVL);
			}
			else
			{
				try
				{
					vecAttributeStringCodeElement.at(j->nPAIX - 1)->appendChild(pAttributeStringCodeElement);
					pAttributeStringCodeElement->Puttext((_bstr_t)j->strATVL);
				}
				catch (std::out_of_range)
				{
					OutputDebugString(_T("PAIX Index overflow\n"));
				}
			}
		}
	}

	return true;
}

CString CClass_S101_InputXML::GetFeatureTypeID(ST_FEATURE_RECORD* pST_FEATURE_RECORD)
{
	CString strID;
	strID.Format(_T("%u"), pST_FEATURE_RECORD->stFRID.nRCID);
	return strID;
}

CString CClass_S101_InputXML::GetInformationTypeID(ST_INFO_RECORD* pST_INFO_RECORD)
{
	CString strID;
	strID.Format(_T("%u"), pST_INFO_RECORD->stIRID.nRCID);
	return strID;
}

CString CClass_S101_InputXML::GetPointID(ST_POINT_RECORD* pST_POINT_RECORD)
{
	CString strID;
	strID.Format(_T("%u"), pST_POINT_RECORD->stPRID.nRCID);
	return strID;
}

CString CClass_S101_InputXML::GetMultiPointID(ST_MULTI_POINT_RECORD* pST_MULTI_POINT_RECORD)
{
	CString strID;
	strID.Format(_T("%u"), pST_MULTI_POINT_RECORD->stMRID.nRCID);
	return strID;
}

CString CClass_S101_InputXML::GetInterpolation(int num)
{
	CString result = L"";
	switch (num)
	{
	case 1:
		result = "Linear";
		break;
	case 2:
		result = "Arc3Points";
		break;
	case 3:
		result = "Geodesic";
		break;
	case 4:
		result = "Loxodromic";
		break;
	case 5:
		result = "Elliptical";
		break;
	case 6:
		result = "Conic";
		break;
	case 7:
		result = "Circular Arc Center Point with Radius";
		break;
	default:
		OutputDebugStringW(L"Invalid Interpolation parameter.");
		break;
	}
	return result;
}

CString CClass_S101_InputXML::GetOrientationFromORNT(unsigned int nORNT)
{
	if (nORNT == 1)
	{
		return _T("Forward");
	}
	else if (nORNT == 2)
	{
		return _T("Reverse");
	}

	return _T("");
}

CString CClass_S101_InputXML::GetSpatialTypeNameFromRRNM(unsigned int nRRNM)
{
	if (nRRNM == 110)
	{
		return _T("Point");
	}
	else if (nRRNM == 115)
	{
		return _T("MultiPoint");
	}
	else if (nRRNM == 120)
	{
		return _T("Curve");
	}
	else if (nRRNM == 125)
	{
		return _T("CompositeCurve");
	}
	else if (nRRNM == 130)
	{
		return _T("Surface");
	}

	return _T("");
}

CString CClass_S101_InputXML::GetFeatureTypePrimitive(ST_FEATURE_RECORD* pST_FEATURE_RECORD)
{
	CString strPrimitive;

	// none 0
	// 110 = point
	// 115 = multi point
	// 120 = curve
	// 125 = composite curve
	// 130 = surface
	// coverage 140
	// complex 150
	std::vector<int> prim;

	for (auto i = pST_FEATURE_RECORD->vecSPAS.begin(); i != pST_FEATURE_RECORD->vecSPAS.end(); i++)
	{
		for (auto j = i->vecRRNM.begin(); j != i->vecRRNM.end(); j++)
		{
			prim.push_back(j->nRRNM);
		}
	}

	if (prim.size() == 0)
	{
		CString strErr;
		strErr.Format(_T("NO SPAS for feature record (id : %d)\n"), pST_FEATURE_RECORD->stFRID.nRCID);
		OutputDebugString(strErr);
	}

	std::sort(prim.begin(), prim.end());
	auto new_end = std::unique(prim.begin(), prim.end());
	prim.resize(std::distance(prim.begin(), new_end));

	if (prim.size() > 1)
	{
		strPrimitive = _T("Complex");
	}
	else if (prim.size() == 1)
	{
		switch (*prim.begin())
		{
		case 110:
			strPrimitive = _T("Point");
			break;
		case 115:
			strPrimitive = _T("MultiPoint");
			break;
		case 120:
		case 125:
			strPrimitive = _T("Curve");
			break;
		case 130:
			strPrimitive = _T("Surface");
			break;
		default:
			OutputDebugString(_T("Error: Invalied prim\n"));
			strPrimitive = _T("None");
			break;
		}
	}
	else if (prim.size() == 0)
	{
		OutputDebugString(_T("Error: There is None type geometry\n"));
		strPrimitive = _T("None");
	}

	return strPrimitive;
}

bool CClass_S101_InputXML::AddParameters(MSXML2::IXSLProcessorPtr pProcessor)
{
	HRESULT hr = pProcessor->addParameter("TWO_SHADES", "false", "");

	if (SUCCEEDED(hr))
	{
		hr = pProcessor->addParameter("SAFETY_DEPTH", "30.0", "");

		if (SUCCEEDED(hr))
		{
			hr = pProcessor->addParameter("SAFETY_CONTOUR", "30.0", "");

			if (SUCCEEDED(hr))
			{
				hr = pProcessor->addParameter("SHALLOW_CONTOUR", "2.0", "");

				if (SUCCEEDED(hr))
				{
					hr = pProcessor->addParameter("DEEP_CONTOUR", "30.0", "");

					if (SUCCEEDED(hr))
					{
						hr = pProcessor->addParameter("SHALLOW_PATTERN", "false", "");

						if (SUCCEEDED(hr))
						{
							hr = pProcessor->addParameter("SHOW_ISOLATED_DANGERS_IN_SHALLOW_WATERS", "false", "");

							if (SUCCEEDED(hr))
							{
								hr = pProcessor->addParameter("FULL_SECTORS", "false", "");

								return true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}