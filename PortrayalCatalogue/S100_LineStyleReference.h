#pragma once
#include "S100_LineStyleBase.h"

#include <pugixml.hpp>

class S100_LineStyleReference : public S100_LineStyleBase
{
public:
	S100_LineStyleReference();
	virtual ~S100_LineStyleReference();

private:
	std::string reference;

public:
	void GetContents(pugi::xml_node& node);

	void SetReference(const std::string& value);
	void SetReference(const std::wstring& value);
	std::string  GetReference();
	std::wstring GetReferenceW();
};
