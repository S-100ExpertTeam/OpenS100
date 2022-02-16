========================================================================
========================================================================
PortrayalCatalogue.vcxproj

PortrayalCatalogue.vcxproj.filters

PortrayalCatalogue.cpp

/////////////////////////////////////////////////////////////////////////////

StdAfx.h, StdAfx.cpp

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"nullInstruction") == 0)

	}