#include "stdafx.h"
#include "S100_ComplexLineStyle.h"

S100_ComplexLineStyle::S100_ComplexLineStyle()
{

}

S100_ComplexLineStyle::~S100_ComplexLineStyle()
{

}

void S100_ComplexLineStyle::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"symbolReference") == 0)
		{
			symbolReference = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"scaleFactor") == 0)
		{
			scaleFactor = std::wstring(pNode->Gettext());
		}
	}
}

void S100_ComplexLineStyle::SetSymbolReference(std::wstring& value)
{
	symbolReference = value;
}

std::wstring S100_ComplexLineStyle::GetSymbolReference()
{
	return symbolReference;
}

void S100_ComplexLineStyle::SetScaleFactor(std::wstring& value)
{
	scaleFactor = value;
}

std::wstring S100_ComplexLineStyle::GetScaleFactor()
{
	return scaleFactor;
}