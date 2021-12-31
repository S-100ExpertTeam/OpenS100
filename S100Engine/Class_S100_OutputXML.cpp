#include "stdafx.h"
#include "Class_S100_OutputXML.h"
#include "DisplayList.h"
#include "DrawingInstructions.h"
#include "DrawingInstructionReader.h"
#include "TextElement.h"
#include "FontCharacteristics.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

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

CClass_S100_OutputXML::CClass_S100_OutputXML()
{

}

CClass_S100_OutputXML::~CClass_S100_OutputXML()
{

}

DrawingInstruction* GetDrawingInstruction(BSTR bsName, BSTR bsValue, DrawingInstruction* result)
{
	CString csTemp;
	if (bsName[0] == 'f')
		csTemp.Format(_T("%s"), bsValue);
	FeatureReference* featureReference = new FeatureReference();
	featureReference->m_strReference = csTemp;
	result->m_pFeatureReference = featureReference;

	return result;
}

TextInstruction* GetTextInstruction(MSXML2::IXMLDOMElementPtr pElementDrawingInstruction)
{
	TextInstruction* result = new TextInstruction();

	MSXML2::IXMLDOMNodeListPtr childNodes = pElementDrawingInstruction->childNodes;

	for (int i = 0; i < childNodes->length; i++)
	{
		MSXML2::IXMLDOMNodePtr currentNode;
		BSTR bsName;
		BSTR bsValue;

		childNodes->get_item(i, &currentNode);
		currentNode->get_nodeName(&bsName);
		currentNode->get_text(&bsValue);

		if (!StrCmpW(bsName, L"featureReference"))
		{
			FeatureReference* featureReference = new FeatureReference();
			featureReference->m_strReference = bsValue;
			result->m_pFeatureReference = featureReference;
		}
		else if (!StrCmpW(bsName, L"textPoint"))
		{
			result->pText = new TextPackage::TextPoint();
			TextPackage::TextPoint* textPoint = (TextPackage::TextPoint*)result->pText;

			MSXML2::IXMLDOMNamedNodeMapPtr attributes;
			currentNode->get_attributes(&attributes);

			for (int j = 0; j < attributes->length; j++)
			{
				MSXML2::IXMLDOMNodePtr currentAttribute;
				attributes->get_item(j, &currentAttribute);

				BSTR bsAttributeName;
				BSTR bsAttributeValue;
				currentAttribute->get_nodeName(&bsAttributeName);
				currentAttribute->get_text(&bsAttributeValue);

				if (!StrCmpW(bsAttributeName, L"horizontalAlignment"))
				{
					if (!StrCmpW(bsAttributeValue, L"Start"))
					{
						textPoint->horizontalAlignment = TextPackage::HorizontalAlignment::start;
					}
					else if (!StrCmpW(bsAttributeValue, L"Right"))
					{
						textPoint->horizontalAlignment = TextPackage::HorizontalAlignment::start;
					}
					else if (!StrCmpW(bsAttributeValue, L"End"))
					{
						textPoint->horizontalAlignment = TextPackage::HorizontalAlignment::end;
					}
					else if (!StrCmpW(bsAttributeValue, L"Left"))
					{
						textPoint->horizontalAlignment = TextPackage::HorizontalAlignment::end;
					}
					else if (!StrCmpW(bsAttributeValue, L"Center"))
					{
						textPoint->horizontalAlignment = TextPackage::HorizontalAlignment::h_center;
					}
				}
				else if (!StrCmpW(bsAttributeName, L"verticalAlignment"))
				{
					if (!StrCmpW(bsAttributeValue, L"Top"))
					{
						textPoint->verticalAlignment = TextPackage::VerticalAlignment::top;
					}
					else if (!StrCmpW(bsAttributeValue, L"Bottom"))
					{
						textPoint->verticalAlignment = TextPackage::VerticalAlignment::bottom;
					}
					else if (!StrCmpW(bsAttributeValue, L"Center"))
					{
						textPoint->verticalAlignment = TextPackage::VerticalAlignment::v_center;
					}
				}
			}

			MSXML2::IXMLDOMNodeListPtr childNodes = currentNode->childNodes;
			for (int j = 0; j < childNodes->length; j++)
			{
				MSXML2::IXMLDOMNodePtr currentNode;
				BSTR bsName;
				BSTR bsValue;

				childNodes->get_item(j, &currentNode);
				currentNode->get_nodeName(&bsName);
				currentNode->get_text(&bsValue);

				if (!StrCmpW(bsName, L"element"))
				{
					MSXML2::IXMLDOMNodeListPtr childNodes = currentNode->childNodes;
					TextPackage::TextElement* textElement = new TextPackage::TextElement();

					for (int k = 0; k < childNodes->length; k++)
					{
						MSXML2::IXMLDOMNodePtr currentNode;
						BSTR bsName;
						BSTR bsValue;

						childNodes->get_item(k, &currentNode);
						currentNode->get_nodeName(&bsName);
						currentNode->get_text(&bsValue);

						if (!StrCmpW(bsName, L"text"))
						{
							textElement->text = bsValue;
						}
						else if (!StrCmpW(bsName, L"bodySize"))
						{
							textElement->bodySize = _ttoi(bsValue);
						}
						else if (!StrCmpW(bsName, L"verticalOffset"))
						{
							textElement->verticalOffset = _wtof(bsValue);
						}
						else if (!StrCmpW(bsName, L"flags"))
						{

						}
						else if (!StrCmpW(bsName, L"foreground"))
						{
						}
						else if (!StrCmpW(bsName, L"background"))
						{
						}
						else if (!StrCmpW(bsName, L"fontCharacteristics"))
						{
							MSXML2::IXMLDOMNodeListPtr childNodes = currentNode->childNodes;

							TextPackage::FontCharacteristics* fontCharacteristics = new TextPackage::FontCharacteristics();

							for (int l = 0; l < childNodes->length; l++)
							{
								MSXML2::IXMLDOMNodePtr currentNode;
								BSTR bsName;
								BSTR bsValue;

								childNodes->get_item(l, &currentNode);
								currentNode->get_nodeName(&bsName);
								currentNode->get_text(&bsValue);

								if (!StrCmpW(bsName, L"weight"))
								{
									if (!StrCmpW(bsValue, L"light"))
									{
										fontCharacteristics->weight = TextPackage::FontWeight::light;
									}
									else if (!StrCmpW(bsValue, L"medium"))
									{
										fontCharacteristics->weight = TextPackage::FontWeight::medium;
									}
									else if (!StrCmpW(bsValue, L"bold"))
									{
										fontCharacteristics->weight = TextPackage::FontWeight::bold;
									}
								}
								else if (!StrCmpW(bsName, L"slant"))
								{
									if (!StrCmpW(bsValue, L"upright"))
									{
										fontCharacteristics->slant = TextPackage::FontSlant::upright;
									}
									else if (!StrCmpW(bsValue, L"italics"))
									{
										fontCharacteristics->slant = TextPackage::FontSlant::italics;
									}
								}
								else if (!StrCmpW(bsName, L"serifs"))
								{

								}
								else if (!StrCmpW(bsName, L"proportion"))
								{

								}
							}

							textElement->font = (TextPackage::Font*)fontCharacteristics;
						}
						else if (!StrCmpW(bsName, L"fontReference"))
						{
						}
					}

					textPoint->element.push_back(textElement);
				}
				else if (!StrCmpW(bsName, L"offset"))
				{
					MSXML2::IXMLDOMNodeListPtr childNodes = currentNode->childNodes;

					for (int k = 0; k < childNodes->length; k++)
					{
						MSXML2::IXMLDOMNodePtr currentNode;
						BSTR bsName;
						BSTR bsValue;

						childNodes->get_item(k, &currentNode);
						currentNode->get_nodeName(&bsName);
						currentNode->get_text(&bsValue);

						if (!StrCmpW(bsName, L"x"))
						{
							if (textPoint->offset)
							{
								textPoint->offset->x = _wtof(bsValue);
							}
							else
							{
								textPoint->offset = new GraphicBasePackage::Vector();
								textPoint->offset->x = _wtof(bsValue);
							}
						}
						else if (!StrCmpW(bsName, L"y"))
						{
							if (textPoint->offset)
							{
								textPoint->offset->y = _wtof(bsValue);
							}
							else
							{
								textPoint->offset = new GraphicBasePackage::Vector();
								textPoint->offset->y = _wtof(bsValue);
							}
						}
					}
				}
				else if (!StrCmpW(bsName, L"areaPlacement"))
				{

				}
			}
		}
		else if (!StrCmpW(bsName, L"TextLine"))
		{
		}
	}
	return result;
}

AreaInstruction* GetAreaInstruction(MSXML2::IXMLDOMElementPtr pElementDrawingInstruction)
{
	AreaInstruction* result = new AreaInstruction();
	result->m_nTypeOfDrawingInstruction = areaInstruction;

	MSXML2::IXMLDOMNodeListPtr pNodeList_areaInstruction = pElementDrawingInstruction->childNodes;
	MSXML2::IXMLDOMNodePtr pNodeValue = NULL;

	for (int j = 0; j < pNodeList_areaInstruction->length; j++)
	{
		BSTR bsValue;
		BSTR bsName;

		pNodeList_areaInstruction->get_item(j, &pNodeValue);
		pNodeValue->get_text(&bsValue);
		pNodeValue->get_nodeName(&bsName);

		GetDrawingInstruction(bsName, bsValue, result);

		if (bsName[0] == 'c')
		{
			MSXML2::IXMLDOMNodePtr pNode;
			pNodeValue->childNodes->get_item(0, &pNode);
			pNode->get_text(&bsValue);
			result->colorName.Format(_T("%s"), bsValue);

			// get transparency attribute
			MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr = NULL;
			pNode->get_attributes(&namedNodeMapPtr);

			long lengthAttributes = 0;

			namedNodeMapPtr->get_length(&lengthAttributes);

			for (auto j = 0; j < lengthAttributes; j++)
			{
				MSXML2::IXMLDOMNodePtr pAttribute = NULL;
				namedNodeMapPtr->get_item(j, &pAttribute);

				if (pAttribute)
				{
					BSTR bstrAttributeName, bstrAttributeValue;
					pAttribute->get_nodeName(&bstrAttributeName);
					pAttribute->get_text(&bstrAttributeValue);

					CString /*strAttributeName, */strAttributeValue;
					if (bstrAttributeName[0] == 't')
					{
						result->transparency = (FLOAT)_ttof(bstrAttributeValue);
						
					}
				}
			}
		}
		else if (bsName[0] == 'a')
		{
			result->colorName = _T("CHBLK");

			IXMLDOMNodePtr pNode;
			pNode = pElementDrawingInstruction->selectSingleNode("areaFillReference");
			VARIANT var;
			((IXMLDOMElementPtr)pNode)->getAttribute(_T("reference"), &var);

			result->patternName = var;
		}
	}
	return result;
}

LineInstruction* GetLineInstruction(MSXML2::IXMLDOMElementPtr pElementDrawingInstruction)
{
	LineInstruction* result = new LineInstruction();
	result->m_nTypeOfDrawingInstruction = lineInstruction;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pElementDrawingInstruction->childNodes;
	MSXML2::IXMLDOMNodePtr pNodeValue = NULL;

	for (int j = 0; j < pNodeList->length; j++)
	{
		BSTR bsValue;
		BSTR bsName;

		pNodeList->get_item(j, &pNodeValue);
		pNodeValue->get_text(&bsValue);
		pNodeValue->get_nodeName(&bsName);

		GetDrawingInstruction(bsName, bsValue, result);

		if (!StrCmpW(bsName, L"lineStyleReference"))
		{
			MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr = NULL;
			pNodeValue->get_attributes(&namedNodeMapPtr);
			long lengthAttributes = 0;

			namedNodeMapPtr->get_length(&lengthAttributes);

			for (auto j = 0; j < lengthAttributes; j++)
			{
				MSXML2::IXMLDOMNodePtr pAttribute = NULL;
				namedNodeMapPtr->get_item(j, &pAttribute);

				if (pAttribute)
				{
					BSTR bstrAttributeName, bstrAttributeValue;
					pAttribute->get_nodeName(&bstrAttributeName);
					pAttribute->get_text(&bstrAttributeValue);

					CString strAttributeName, strAttributeValue;

					strAttributeName.Format(_T("%s"), bstrAttributeName);

					if (!strAttributeName.CompareNoCase(_T("reference")))
					{
						result->lineStyleReference = bstrAttributeValue;
					}
				}
			}
		}
		else if (!StrCmpW(bsName, L"lineStyle"))
		{
			result->colorName = _T("CHBLK");

			MSXML2::IXMLDOMNodeListPtr pChildNodeList;
			pNodeValue->get_childNodes(&pChildNodeList);
			for (auto i = 0; i < pChildNodeList->length; i++)
			{
				MSXML2::IXMLDOMNodePtr pChildNodeValue = nullptr;
				pChildNodeList->get_item(i, &pChildNodeValue);

				if (pChildNodeValue)
				{
					BSTR bsChildName;
					pChildNodeValue->get_nodeName(&bsChildName);

					if (!StrCmpW(bsChildName, L"pen"))
					{
						BSTR bsChildText;
						pChildNodeValue->GetfirstChild()->get_text(&bsChildText);

						result->colorName = bsChildText;

						// get width attribute
						MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr = NULL;
						pChildNodeValue->get_attributes(&namedNodeMapPtr);

						long lengthAttributes = 0;

						namedNodeMapPtr->get_length(&lengthAttributes);

						for (auto j = 0; j < lengthAttributes; j++)
						{
							MSXML2::IXMLDOMNodePtr pAttribute = NULL;
							namedNodeMapPtr->get_item(j, &pAttribute);

							if (pAttribute)
							{
								BSTR bstrAttributeName, bstrAttributeValue;
								pAttribute->get_nodeName(&bstrAttributeName);
								pAttribute->get_text(&bstrAttributeValue);

								CString strAttributeName, strAttributeValue;

								strAttributeName.Format(_T("%s"), bstrAttributeName);

								if (!strAttributeName.CompareNoCase(_T("width")))
								{
									result->width = _ttof(bstrAttributeValue);
								}
							}
						}
					}
					else if (!StrCmpW(bsChildName, L"dash"))
					{
						result->hasDash = true;
					}
				}
			}
		}
		else
		{
			result->width = 0.32;
			result->colorName = _T("CHBLK");
		}
	}
	return result;
}

NullInstruction* GetNullInstruction(MSXML2::IXMLDOMElementPtr pElementDrawingInstruction)
{
	NullInstruction* result = new NullInstruction();
	result->m_nTypeOfDrawingInstruction = nullInstruction;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pElementDrawingInstruction->childNodes;
	MSXML2::IXMLDOMNodePtr pNodeValue = NULL;
	for (int j = 0; j < pNodeList->length; j++)
	{
		BSTR bsValue;
		BSTR bsName;

		pNodeList->get_item(j, &pNodeValue);
		pNodeValue->get_text(&bsValue);
		pNodeValue->get_nodeName(&bsName);
		CString csTemp;
		GetDrawingInstruction(bsName, bsValue, result);
	}
	return result;
}

PointInstruction* GetPointInstruction(MSXML2::IXMLDOMElementPtr pElementDrawingInstruction)
{
	PointInstruction* result = new PointInstruction();

	result->m_nTypeOfDrawingInstruction = pointInstruction;
	MSXML2::IXMLDOMNodeListPtr pNodeList = pElementDrawingInstruction->childNodes;
	MSXML2::IXMLDOMNodePtr pNodeValue = NULL;

	for (auto i = 0; i < pNodeList->length; i++)
	{
		BSTR bsValue;
		BSTR bsName;

		pNodeList->get_item(i, &pNodeValue);
		pNodeValue->get_text(&bsValue);
		pNodeValue->get_nodeName(&bsName);
		CString csTemp;
		GetDrawingInstruction(bsName, bsValue, result);


		if (bsName[0] == 's')
		{
			MSXML2::IXMLDOMNamedNodeMapPtr namedNodeMapPtr = NULL;
			pNodeValue->get_attributes(&namedNodeMapPtr);

			long lengthAttributes = 0;

			namedNodeMapPtr->get_length(&lengthAttributes);

			for (auto j = 0; j < lengthAttributes; j++)
			{
				MSXML2::IXMLDOMNodePtr pAttribute = NULL;
				namedNodeMapPtr->get_item(j, &pAttribute);

				if (pAttribute)
				{
					BSTR bstrAttributeName, bstrAttributeValue;
					pAttribute->get_nodeName(&bstrAttributeName);
					pAttribute->get_text(&bstrAttributeValue);

					CString strAttributeName, strAttributeValue;

					strAttributeName.Format(_T("%s"), bstrAttributeName);

					if (strAttributeName.GetLength() == 9)
					{
						result->m_pSymbol = NULL;
						result->symbolName = bstrAttributeValue;
					}
					else if (strAttributeName.GetLength() == 8)
					{
						result->rotation = (FLOAT)_ttoi(bstrAttributeValue);
					}
				}
			}
		}
	}
	return result;
}


DisplayList* CClass_S100_OutputXML::OpenOutputXMLbypugixml(CString path)
{
	pugi::xml_document doc;
	char* charPath = LibMFCUtil::ConvertWCtoC((wchar_t*)std::wstring(path).c_str());
	pugi::xml_parse_result result = doc.load_file(charPath);
	delete[] charPath;
	charPath = nullptr;

	if (!result)
		return nullptr;

	pugi::xml_node displayList = doc.child("displayList");

	if (!displayList)
		return nullptr;

	DisplayList* pDisplayList = new DisplayList();

	for (pugi::xml_node instruction = displayList.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "pointInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetPointInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "lineInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetLineInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "areaInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAreaInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "textInstruction"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetTextInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "augmentedPath"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAugmentedPathInstructionBypugixml(instruction));
		}
		else if (!strcmp(instructionName, "augmentedRay"))
		{
			pDisplayList->m_vecInstruction.push_back(DrawingInstructionReader::GetAugmentedRayInstructionBypugixml(instruction));
		}
	}

	return pDisplayList;
}



DisplayList* CClass_S100_OutputXML::OpenOutputXML(CString path)
{
	HRESULT hr = ::CoInitialize(NULL);

	if (!SUCCEEDED(hr))
	{
		OutputDebugString(_T("Failed to call CoInitialize()\n"));
		return nullptr;
	}

	DisplayList* pDisplayList = new DisplayList();

	MSXML2::IXMLDOMDocument2Ptr pOutputXML = NULL;

	// 1ms
	if (SUCCEEDED(pOutputXML.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument))) && pOutputXML)
	{
		if (SUCCEEDED(pOutputXML->put_async(VARIANT_FALSE)))
		{
					// 37ms
			if (pOutputXML->load(_variant_t(path)))
			{
				MSXML2::IXMLDOMElementPtr pRoot;

				if (SUCCEEDED(pOutputXML->get_documentElement(&pRoot)))
				{
					MSXML2::IXMLDOMNodePtr child;
					pRoot->get_firstChild(&child);

					while (child)
					{
						BSTR nodeName;
						child->get_nodeName(&nodeName);
						MSXML2::IXMLDOMElementPtr element = child;

						if (!StrCmpW(nodeName, _T("areaInstruction")))
						{
							pDisplayList->m_vecInstruction.push_back(GetAreaInstruction(element));
						}
						else if (!StrCmpW(nodeName, _T("lineInstruction")))
						{
							pDisplayList->m_vecInstruction.push_back(GetLineInstruction(element));
						}
						else if (!StrCmpW(nodeName, _T("pointInstruction")))
						{
							pDisplayList->m_vecInstruction.push_back(GetPointInstruction(element));
						}
						else if (!StrCmpW(nodeName, _T("textInstruction")))
						{
							pDisplayList->m_vecInstruction.push_back(GetTextInstruction(element));
						}

						child->get_nextSibling(&child);
					}
				}
			}
		}
	}

	if (!SUCCEEDED(hr))
	{
		::CoUninitialize();
		return nullptr;
	}
	return pDisplayList;
}