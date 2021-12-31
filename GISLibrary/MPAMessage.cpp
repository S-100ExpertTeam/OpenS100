#include "stdafx.h"
#include "MPAMessage.h"
#include "GML_Attribute.h"
#include "GML_Feature.h"
#include "ATTF.h"

#include "..\\PLLibrary\\CSymbol.h"

//#include "LayerManager.h"
MPAMessage::MPAMessage(void)
{
}


MPAMessage::~MPAMessage(void)
{
	unordered_map<LPCWSTR, GML_Feature*>::iterator itor;

	for(itor = m_features.begin(); itor != m_features.end(); itor++)
	{
		delete itor->second;
	}

	m_features.clear();

}

bool MPAMessage::Open(CString _filepath)
{
	SetFilePath(_filepath);
	int indexOfSplit = _filepath.ReverseFind('\\');
	//m_FilePath = _filepath.Mid(indexOfSplit + 1);
	m_FileType = FILE_GML; //TEXT("GML");

	// xml 오픈
	MSXML2::IXMLDOMDocument2Ptr g_pXMLDoc;				// XML Document 선언
	MSXML2::IXMLDOMNodeListPtr pNodeList;				// 찾은 catalogInfo 노드들

	if (!_filepath.IsEmpty())
	{
		g_pXMLDoc.CreateInstance(__uuidof(DOMDocument));
		g_pXMLDoc->async = VARIANT_FALSE;

		g_pXMLDoc->validateOnParse = VARIANT_TRUE;
		g_pXMLDoc->resolveExternals = VARIANT_TRUE;
		// 인스턴스 생성F
		g_pXMLDoc->load((_variant_t)_filepath);
		//aDEPARE, aDRGARE, aFLODOC, aHULKES, aLNDARE, aPONTON, aUNSARE
		//lDEPCNT
		pNodeList = g_pXMLDoc->selectNodes(L"//MPA:DataSet");
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


void MPAMessage::GetENCObject(MSXML2::IXMLDOMNodePtr listPtr)
{
	USES_CONVERSION;
	MSXML2::IXMLDOMNodeListPtr pNodeList = listPtr->GetchildNodes();

	int cnt = pNodeList->Getlength();
	for(int i = 0; i < cnt; i++) {
		MSXML2::IXMLDOMElementPtr item = pNodeList->Getitem(i);
		if(item == NULL)
			continue;

		// 1. Get Metadata 
		std::string str = std::string(W2A(item->baseName));

		MSXML2::IXMLDOMNodeListPtr pChildNodeList = item->GetchildNodes();

		if(str.compare("DatasetMetadataProperty") == 0)
		{
			int cnt2 = pChildNodeList->Getlength();
			for(int i = 0; i < cnt2; i++) {
				MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
				if(pChildItem == NULL)
					continue;				
				std::string str2 = std::string(W2A(pChildItem->baseName));
				if(str2.compare("DatasetMetadata") == 0)
				{
					//MSXML2::IXMLDOMNodeListPtr pChildChildNodeList = pChildItem->GetchildNodes();
					GetDatasetMetadata(pChildItem);
				}
			}
		}else if(str.compare("boundedBy") == 0)
		{
			int cnt2 = pChildNodeList->Getlength();
			for(int i = 0; i < cnt2; i++) {
				MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
				if(pChildItem == NULL)
					continue;				
				std::string str2 = std::string(W2A(pChildItem->baseName));
				if(str2.compare("Envelope") == 0)
				{
					//MSXML2::IXMLDOMNodeListPtr pChildChildNodeList = pChildItem->GetchildNodes();
					m_boundedBy.GetDataFromXML(pChildItem);
				}
			}
		}
		else if(str.compare("member") == 0)
		{
			int cnt2 = pChildNodeList->Getlength();
			for(int i = 0; i < cnt2; i++) {
				MSXML2::IXMLDOMElementPtr pChildItem = pChildNodeList->Getitem(i);
				if(pChildItem == NULL)
					continue;				
				std::string str2 = std::string(W2A(pChildItem->baseName));
				if(str2.compare("MarineProtectedArea") == 0)
				{
					MSXML2::IXMLDOMNodeListPtr pChildChildNodeList = pChildItem->GetchildNodes();

					GML_Feature* pFe = new GML_Feature();
					_variant_t id = pChildItem->getAttribute("gml:id");
					m_features.insert(unordered_map<LPCWSTR, GML_Feature*>::value_type(id.bstrVal, pFe));

					id.ChangeType(VT_BSTR);
					pFe->m_name = id.bstrVal;

					// Fixed Object Code to 30501.
					unsigned scaleMin = 0;
					int simpleIndex = 0;
					int paperIndex = 0; 
					CSymbol* symbol = NULL;
					CObjectFindStruct* obj = NULL;

					pFe->m_objlCode = 30501;

					int cnt3 = pChildChildNodeList->Getlength();
					for(int i = 0; i < cnt3; i++) {

						MSXML2::IXMLDOMElementPtr pChildChildItem = pChildChildNodeList->Getitem(i);
						if(pChildItem == NULL)
							continue;				
						//////////////////////////////////////////////////
						//// devided feature by spatial type
						//list<GML_Feature*> m_PRFeature;
						//list<GML_Feature*> m_MPRFeature;
						//list<GML_Feature*> m_CCFeature;
						//list<GML_Feature*> m_SRFeature;
						//////////////////////////////////////////////////

						std::string str3 = std::string(W2A(pChildChildItem->baseName));

						if(		str3.compare("surfaceProperty") == 0 
							|| str3.compare("lineProperty") == 0 
							|| str3.compare("pointProperty") == 0 
							)
						{
							if(!obj)
								obj = new CObjectFindStruct();
							obj->m_objectCode = pFe->m_objlCode;
							// 속성정의에 따라 OBJECT NAME과 CATEGORY 설정
							list<GML_Attribute*>::iterator attritor;
							for(attritor = pFe->m_attributes.begin(); attritor != pFe->m_attributes.end(); attritor++)
							{
								GML_Attribute* attr = *attritor;
								if(attr->m_name.compare(L"name") == 0)
								{
									list<GML_Attribute*>::iterator attritor2;
									for(attritor2 = attr->m_attributes.begin(); attritor2 != attr->m_attributes.end(); attritor2++)
									{
										GML_Attribute* attr2 = *attritor2;
										if(attr2->m_name.compare(L"text") == 0)
										{
											ATTF* attf = new ATTF();
											attf->ATTL = 116;
											attf->ATVL = attr2->m_value;
											pFe->m_attfList.push_back(attf);
										}
									}
								}
							}

							ATTF *attf;
							LISTATTF::iterator itorAttf;
							for( itorAttf = pFe->m_attfList.begin(); itorAttf != pFe->m_attfList.end(); ++itorAttf )
							{
								attf = *itorAttf;
								CAttributeFindStruct attrFind;
								attrFind.m_code = &attf->ATTL;
								//attrFind.m_value.Format(attf->ATVL.c_str()); cause error by '%'
								attrFind.m_value = attf->ATVL.c_str();
								obj->m_attrList.AddTail(attrFind);

								if(attf->ATTL == 133) // Attribute : Scale Min 
								{
									scaleMin = _wtoi(attf->ATVL.c_str());
								}
							}
						}
						
						if(	str3.compare("surfaceProperty") == 0)
						{
							m_SRFeature.push_back(pFe);
							pFe->m_type = 3;
							pFe->m_GMLGeoData = MakeFullSpatialData(pChildChildItem);

							simpleIndex = ENCGeometry::pSymbol[3].GetIndexNumber(obj);
							paperIndex = ENCGeometry::pSymbol[4].GetIndexNumber(obj);
							symbol = ENCGeometry::pSymbol[4].GetSymbole(paperIndex);

							pFe->m_displayPriority = symbol->Field4;
							pFe->m_radarOver = symbol->Field5;		
							pFe->m_displayCategory = symbol->Field6;
							pFe->m_viewingGroup = symbol->Field7;

							pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
							pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
							pFe->m_GMLGeoData->m_geo->SetAttribute(obj);
						}
						else if(str2.compare("lineProperty") == 0)
						{
							m_CCFeature.push_back(pFe);
							pFe->m_type = 2;
							pFe->m_GMLGeoData = MakeFullSpatialData(pChildChildItem);
							simpleIndex = ENCGeometry::pSymbol[2].GetIndexNumber(obj);
							paperIndex = simpleIndex;
							symbol = ENCGeometry::pSymbol[2].GetSymbole(simpleIndex);

							pFe->m_displayPriority = symbol->Field4;
							pFe->m_radarOver = symbol->Field5;		
							pFe->m_displayCategory = symbol->Field6;
							pFe->m_viewingGroup = symbol->Field7;

							pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
							pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
							pFe->m_GMLGeoData->m_geo->SetAttribute(obj);
						}
						else if(str2.compare("pointProperty") == 0)
						{
							m_PRFeature.push_back(pFe);
							pFe->m_type = 1;
							pFe->m_GMLGeoData = MakeFullSpatialData(pChildChildItem);
							simpleIndex = ENCGeometry::pSymbol[0].GetIndexNumber(obj);
							paperIndex = ENCGeometry::pSymbol[1].GetIndexNumber(obj);
							symbol = ENCGeometry::pSymbol[0].GetSymbole(simpleIndex);

							pFe->m_displayPriority = symbol->Field4;
							pFe->m_radarOver = symbol->Field5;		
							pFe->m_displayCategory = symbol->Field6;
							pFe->m_viewingGroup = symbol->Field7;

							pFe->m_GMLGeoData->m_geo->SetPLIndex(simpleIndex, paperIndex);			
							pFe->m_GMLGeoData->m_geo->SetScaleMin(scaleMin);
							pFe->m_GMLGeoData->m_geo->SetAttribute(obj);
						}
						else 
						{
							GetFeatureType(pChildChildItem, pFe);
						}
					}
				}
			}
		}
	}
	return;
}