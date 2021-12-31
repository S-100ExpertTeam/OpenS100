#include "stdafx.h"
#include "S100_SymbolFill.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"

S100_SymbolFill::S100_SymbolFill()
{
	clipSymbols = L"true";
	symbol = NULL;

	v1 = NULL;
	v2 = NULL;
	
	SetType(4);
}

S100_SymbolFill::~S100_SymbolFill()
{
	if (symbol) delete symbol;

	if (v1) delete v1;
	if (v2) delete v2;
}

bool S100_SymbolFill::ReadFile(std::wstring& path)
{
	
	std::wstring head = L"..\\ProgramData\\S101_Portrayal\\AreaFills\\";

	path.insert(path.begin(), head.begin(), head.end());

	return ReadFileByPugiXml(path);

	MSXML2::IXMLDOMDocument2Ptr pDoc;							// XML Document
	MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap;					// for attribute
	MSXML2::IXMLDOMNodeListPtr pNodeInfo;						// find node List
	MSXML2::IXMLDOMNodePtr pNode;								

	if (!path.empty())
	{
		pDoc.CreateInstance(__uuidof(DOMDocument));  
		VARIANT_BOOL ret = pDoc->load((_variant_t)path.c_str());
		if (!ret)
		{
			return false;
		}

		MSXML2::IXMLDOMNodeListPtr pChild = pDoc->childNodes;

		for (int i = 0; i < pChild->Getlength(); i++)
		{
			MSXML2::IXMLDOMNodePtr pNode = pChild->Getitem(i);

			std::wstring intervalLength;
			std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

			if (nodeName.compare(L"symbolFill") == 0)
			{
				GetContents(pNode);
			}
		}
	}
	return true;
}
bool S100_SymbolFill::ReadFileByPugiXml(std::wstring& path)
{
	if (!path.empty())
	{
		 LibMFCUtil::GetFileName(path.c_str());

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str());
		pugi::xml_node displayList = doc.child("symbolFill");

		if (displayList)
		{
			GetContents(displayList);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void S100_SymbolFill::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
	{
		return;
	}
	((S100_PatternFill*)this)->GetContents(pNode);
	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT value;

	pAttrNP = pAttr->getNamedItem(L"clipSymbols");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		clipSymbols = std::wstring(value.bstrVal).c_str();
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();
	
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode) 
		{
			continue;
		}
		

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"symbol") == 0)
		{
			if (!symbol)
			{
				symbol = new S100_Symbol();
				symbol->GetContents(pNode);
			}
		
		}
		else if (nodeName.compare(L"v1") == 0)
		{
			if (!v1) 
			{
				v1 = new S100_VectorPoint();
				v1->GetContents(pNode);
			}
			
		}
		else if (nodeName.compare(L"v2") == 0)
		{
			if (!v2)
			{
				v2 = new S100_VectorPoint();
				v2->GetContents(pNode);
			}
		}
	}
}
void S100_SymbolFill::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}
	
	auto Attribute = node.attribute("clipSymbols"); //save attribute
	if (Attribute) 
	{
		clipSymbols =pugi::as_wide(Attribute.value());
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "symbol"))
		{
			if (!symbol)
			{
				symbol = new S100_Symbol();
				symbol->GetContents(instruction);
			}
		}
		else if (!strcmp(instructionName, "v1"))
		{
			if (!v1)
			{
				v1 = new S100_VectorPoint();
				v1->GetContents(instruction);
			}
		}
		else if (!strcmp(instructionName, "v2"))
		{
			if (!v2)
			{
				v2 = new S100_VectorPoint();
				v2->GetContents(instruction);
			}
		}

	}
}

void S100_SymbolFill::SetClipSymbols(std::wstring& value)
{
	clipSymbols = value;
}
void S100_SymbolFill::SetSymbol(S100_Symbol* value) 
{
	symbol = value;
}
void S100_SymbolFill::SetV1(S100_VectorPoint* value) 
{
	v1 = value;
}
void S100_SymbolFill::SetV2(S100_VectorPoint* value)
{
	v2 = value;
}

std::wstring S100_SymbolFill::GetClipSymbols()
{
	return clipSymbols;
}
S100_Symbol* S100_SymbolFill::GetSymbol()
{
	return symbol;
}
S100_VectorPoint* S100_SymbolFill::GetV1()
{
	return v1;
}
S100_VectorPoint* S100_SymbolFill::GetV2()
{
	return v2;
}