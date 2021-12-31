#include "stdafx.h"
#include "S100_CallTemplate.h"

S100_CallTemplate::S100_CallTemplate()
{

}

S100_CallTemplate::~S100_CallTemplate()
{
	for (auto itor = params.begin(); itor != params.end(); itor++)
		delete *itor;
}

void S100_CallTemplate::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"name");
	VARIANT value;
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		name = std::wstring(value.bstrVal);
	}
	
	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"xsl:with-param") == 0)
		{
			S100_WithParam *cont = new S100_WithParam();
			params.push_back(cont);

			cont->GetContents(pNode);
		}
	}
}

void S100_CallTemplate::SetName(std::wstring& value)
{
	name = value;
}

void S100_CallTemplate::SetParam(S100_WithParam* value) 
{
	params.push_back(value);
}

void S100_CallTemplate::SetParams(std::vector<S100_WithParam*> value) 
{
	params = value;
}

std::wstring S100_CallTemplate::GetName() 
{
	return name;
}

S100_WithParam* S100_CallTemplate::GetParam(int index) 
{
	return params.at(index);
}

std::vector<S100_WithParam*> S100_CallTemplate::GetParams() 
{
	return params;
}