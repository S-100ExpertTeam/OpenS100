#pragma once
#include <string>
class ProcessS101XSLT
{
public:
	ProcessS101XSLT();
	~ProcessS101XSLT();

	static int ProcessS101XSLT2XML(std::wstring xsltPath, std::wstring inXMLPath, std::wstring outXMLPath);
};

