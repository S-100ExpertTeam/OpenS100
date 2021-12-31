#pragma once
#include "S100_LineStyleBase.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

class S100_LineStyleReference : public S100_LineStyleBase
{
public:
	S100_LineStyleReference();
	virtual ~S100_LineStyleReference();

private:
	std::wstring reference = L"";

public:
	void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	void SetReference(std::wstring& value);
	std::wstring GetReference();
};