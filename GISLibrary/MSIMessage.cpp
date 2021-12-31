#include "StdAfx.h"
#include "MSIMessage.h"
MSIMessage::MSIMessage(void)
{
}


MSIMessage::~MSIMessage(void)
{
	unordered_map<LPCWSTR, GML_Feature*>::iterator itor;

	for(itor = m_features.begin(); itor != m_features.end(); itor++)
	{
		delete itor->second;
	}

	m_features.clear();
}

bool MSIMessage::Open(CString _filepath)
{
	//int indexOfSplit = _filepath.ReverseFind('\\');
	SetFilePath(_filepath);
	//m_FilePath = _filepath.Mid(indexOfSplit + 1);
	m_FileType = FILE_GML;//TEXT("GML");

	// xml 오픈
	MSXML2::IXMLDOMDocument2Ptr g_pXMLDoc;				// XML Document 선언
	MSXML2::IXMLDOMNodeListPtr pNodeList;				// 찾은 catalogInfo 노드들

	if (!_filepath.IsEmpty())
	{
		g_pXMLDoc.CreateInstance(__uuidof(DOMDocument));
		g_pXMLDoc->async = VARIANT_FALSE;

		g_pXMLDoc->validateOnParse = VARIANT_TRUE;
		g_pXMLDoc->resolveExternals = VARIANT_TRUE;
		// 인스턴스 생성
		g_pXMLDoc->load((_variant_t)_filepath);
		//aDEPARE, aDRGARE, aFLODOC, aHULKES, aLNDARE, aPONTON, aUNSARE
		//lDEPCNT
		pNodeList = g_pXMLDoc->selectNodes(L"//NoticeBatch");
		{
			for(int i=0; i<pNodeList->Getlength(); i++) {
				MSXML2::IXMLDOMNodePtr pNodePtr = pNodeList->Getitem(i);
				GetENCObject(pNodePtr);
			}
		}
		pNodeList.Release();
	}
		
	SortByFeatureType();
	//MakeFullSpatialData();
	CalcMBR();
	
	m_pLayer->m_mbr.xmin = m_mbr.xmin;
	m_pLayer->m_mbr.xmax = m_mbr.xmax;
	m_pLayer->m_mbr.ymin = m_mbr.ymin;
	m_pLayer->m_mbr.ymax = m_mbr.ymax;

	return true;
}




void MSIMessage::GetENCObject(MSXML2::IXMLDOMNodePtr listPtr)
{
	//USES_CONVERSION;
	//MSXML2::IXMLDOMNodeListPtr pNodeList = listPtr->GetchildNodes();

	//int cnt = pNodeList->Getlength();
	//for(int i = 0; i < cnt; i++) {
	//	MSXML2::IXMLDOMElementPtr item = pNodeList->Getitem(i);
	//	if(item == NULL)
	//		continue;
	//
	//	// 1. Get Metadata 
	//	std::string str = std::string(W2A(item->baseName));
	//	
	//	MSXML2::IXMLDOMNodeListPtr pChildNodeList = item->GetchildNodes();

	//	if(str.compare("DatasetMetadataProperty") == 0)
	//	{
	//		int cnt2 = pChildNodeList->Getlength();
	//		for(int i = 0; i < cnt2; i++) {
	//			MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
	//			if(pChildItem == NULL)
	//				continue;				
	//			std::string str2 = std::string(W2A(pChildItem->baseName));
	//			if(str2.compare("DatasetMetadata") == 0)
	//			{
	//				//MSXML2::IXMLDOMNodeListPtr pChildChildNodeList = pChildItem->GetchildNodes();
	//				GetDatasetMetadata(pChildItem);
	//			}
	//		}
	//	}else if(str.compare("gml:boundedBy") == 0)
	//	{
	//		int cnt2 = pChildNodeList->Getlength();
	//		for(int i = 0; i < cnt2; i++) {
	//			MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
	//			if(pChildItem == NULL)
	//				continue;				
	//			std::string str2 = std::string(W2A(pChildItem->baseName));
	//			if(str2.compare("DatasetMetadata") == 0)
	//			{
	//				//MSXML2::IXMLDOMNodeListPtr pChildChildNodeList = pChildItem->GetchildNodes();
	//				GetDatasetMetadata(pChildItem);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		std::wstring code = item->baseName;
	//		FeatureType* ft = (LayerManager::p10XMSICatalog.6m_pCatalogueList.begin()->second)->GetFeatureType(code);
	//		if(ft != NULL)
	//		{
	//			GML_Feature* pFe = new GML_Feature();
	//			_variant_t id = item->getAttribute("gml:id");

	//			id.ChangeType(VT_BSTR);
	//			pFe->m_name = item->baseName;
	//			pFe->m_id = id.bstrVal;
	//			m_features.insert(unordered_map<LPCWSTR, GML_Feature*>::value_type(id.bstrVal, pFe));
	//			
	//			// Fixed Object Code to 40000.
	//			unsigned scaleMin = 0;
	//			int simpleIndex = 0;
	//			int paperIndex = 0; 
	//			CSymbol* symbol = NULL;
	//			CObjectFindStruct obj;

	//			pFe->m_objlCode = 40000;
	//			obj.m_objectCode = pFe->m_objlCode;

	//			ATTF *attf;
	//			LISTATTF::iterator itorAttf;
	//			for( itorAttf = pFe->m_attfList.begin(); itorAttf != pFe->m_attfList.end(); ++itorAttf )
	//			{
	//				attf = *itorAttf;
	//				CAttributeFindStruct attrFind;
	//				attrFind.m_code = &attf->ATTL;
	//				//attrFind.m_value.Format(attf->ATVL.c_str()); cause error by '%'
	//				attrFind.m_value = attf->ATVL.c_str();
	//				obj.m_attrList.AddTail(attrFind);

	//				if(attf->ATTL == 133) // Attribute : Scale Min 
	//				{
	//					scaleMin = _wtoi(attf->ATVL.c_str());
	//				}
	//			}

	//			int cnt2 = pChildNodeList->Getlength();
	//			for(int i = 0; i < cnt2; i++) {

	//				MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
	//				if(pChildItem == NULL)
	//					continue;
	//				//////////////////////////////////////////////////
	//				//// devided feature by spatial type
	//				//list<GML_Feature*> m_PRFeature;
	//				//list<GML_Feature*> m_MPRFeature;
	//				//list<GML_Feature*> m_CCFeature;
	//				//list<GML_Feature*> m_SRFeature;
	//				//////////////////////////////////////////////////

	//				std::string str2 = std::string(W2A(pChildItem->baseName));
	//				if(	str2.compare("surfaceProperty") == 0
	//					)
	//				{
	//					m_SRFeature.push_back(pFe);
	//					pFe->m_type = 3;
	//					pFe->m_GMLGeoData = MakeFullSpatialData(pChildItem);

	//					simpleIndex = ENCGeometry::pSymbol[3].GetIndexNumber(&obj);
	//					paperIndex = ENCGeometry::pSymbol[4].GetIndexNumber(&obj);
	//					symbol = ENCGeometry::pSymbol[4].GetSymbole(paperIndex);

	//					pFe->m_displayPriority = symbol->Field4;
	//					pFe->m_radarOver = symbol->Field5;		
	//					pFe->m_displayCategory = symbol->Field6;
	//					pFe->m_viewingGroup = symbol->Field7;

	//					pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
	//					pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
	//				}
	//				else if(str2.compare("lineProperty") == 0)
	//				{
	//					m_CCFeature.push_back(pFe);
	//					pFe->m_type = 2;
	//					pFe->m_GMLGeoData = MakeFullSpatialData(pChildItem);
	//					simpleIndex = ENCGeometry::pSymbol[2].GetIndexNumber(&obj);
	//					paperIndex = simpleIndex;
	//					symbol = ENCGeometry::pSymbol[2].GetSymbole(simpleIndex);

	//					pFe->m_displayPriority = symbol->Field4;
	//					pFe->m_radarOver = symbol->Field5;		
	//					pFe->m_displayCategory = symbol->Field6;
	//					pFe->m_viewingGroup = symbol->Field7;

	//					pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
	//					pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
	//				}
	//				else if(str2.compare("pointProperty") == 0)
	//				{
	//					m_PRFeature.push_back(pFe);
	//					pFe->m_type = 1;
	//					pFe->m_GMLGeoData = MakeFullSpatialData(pChildItem);

	//					if(pFe->m_GMLGeoData)
	//					{
	//						simpleIndex = ENCGeometry::pSymbol[0].GetIndexNumber(&obj);
	//						paperIndex = ENCGeometry::pSymbol[1].GetIndexNumber(&obj);
	//						symbol = ENCGeometry::pSymbol[1].GetSymbole(simpleIndex);

	//						pFe->m_displayPriority = symbol->Field4;
	//						pFe->m_radarOver = symbol->Field5;		
	//						pFe->m_displayCategory = symbol->Field6;
	//						pFe->m_viewingGroup = symbol->Field7;

	//						pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
	//						pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
	//					}
	//				}
	//				else 
	//				{
	//					GetFeatureType(pChildItem, pFe);
	//				}
	//			}
	//		}
	//	}
	//}
	return;
}