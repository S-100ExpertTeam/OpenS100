#pragma once
#include "S100_Color.h"

#include "..\\S100Engine\\Color.h"
#include <pugixml.hpp>

#include <string>

class S100_Pen
{
public:
	S100_Pen();
	virtual ~S100_Pen();

private:
	std::string width;
	GraphicBasePackage::Color color;

public:
	void GetContents(pugi::xml_node& node);

	void SetWidth(const std::string& value);
	void SetWidth(const std::wstring& value);
	std::string  GetWidth();
	std::wstring GetWidthW();
	void SetColor(GraphicBasePackage::Color value);
	GraphicBasePackage::Color& GetColor();
};
