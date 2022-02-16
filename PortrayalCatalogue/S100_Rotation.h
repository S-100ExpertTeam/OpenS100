#pragma once

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_Rotation
{
public:
	S100_Rotation();
	virtual ~S100_Rotation();

private:
	std::wstring useValueOf;
	std::wstring value;

public:
	void GetContents(pugi::xml_node& node);

	void SetUseValueOf(std::wstring& value);
	void SetValue(std::wstring& _value);

	std::wstring GetUseValueOf();
	std::wstring GetValue();
};