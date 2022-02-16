#pragma once
#include "S100_Text.h"
#include "S100_Foreground.h"
#include "S100_Font.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class S100_Element
{
public:
	S100_Element();
	virtual ~S100_Element();

private:
	S100_Text *text;
	std::wstring bodySize;
	std::wstring verticalOffset;
	S100_Foreground* foreground;
	S100_Font* font;

public:
	void SetText(S100_Text* value);
	S100_Text* GetText();

	void SetBodySize(std::wstring& value);
	std::wstring GetBodySize();

	void SetVerticalOffset(std::wstring& value);
	std::wstring GetVerticalOffset();

	void SetForground(S100_Foreground* value);
	S100_Foreground* GetForground();

	void SetFont(S100_Font* value);
	S100_Font* GetFont();
	void GetContents(pugi::xml_node node);
};