#pragma once
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class S100_Text
{
public:
	S100_Text();
	virtual ~S100_Text();

private:
	std::wstring value;
	std::wstring useValueOf;

public:
	void GetContents(pugi::xml_node node);

	void SetValue(std::wstring& _value);
	void SetUseValueOf(std::wstring& value);

	std::wstring GetValue();
	std::wstring GetUseValueOf();
};