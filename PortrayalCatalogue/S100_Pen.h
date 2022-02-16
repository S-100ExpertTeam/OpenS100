#pragma once
#include "S100_Color.h"

#include "..\\S100Engine\\Color.h"
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class S100_Pen
{
public:
	S100_Pen();
	virtual ~S100_Pen();

private:
	std::wstring width;
	GraphicBasePackage::Color color;

public: 
	void GetContents(pugi::xml_node& node);

	void SetWidth(std::wstring& value);
	std::wstring GetWidth();
	void SetColor(GraphicBasePackage::Color value);
	GraphicBasePackage::Color& GetColor();
};