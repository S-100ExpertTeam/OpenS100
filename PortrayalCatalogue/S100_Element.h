#pragma once
#include "S100_Text.h"
#include "S100_Foreground.h"
#include "S100_Font.h"

#include <pugixml.hpp>

// Text Package : TextElement
class S100_Element
{
public:
	S100_Element();
	virtual ~S100_Element();

private:
	S100_Text* text = false;
	std::string bodySize;
	std::string verticalOffset;
	S100_Foreground* foreground = false;
	S100_Font* font = false;

public:
	void SetText(S100_Text* value);
	S100_Text* GetText();

	void SetBodySize(const std::string& value);
	void SetBodySize(const std::wstring& value);
	std::string  GetBodySize();
	std::wstring GetBodySizeW();

	void SetVerticalOffset(const std::string& value);
	void SetVerticalOffset(const std::wstring& value);
	std::string  GetVerticalOffset();
	std::wstring GetVerticalOffsetW();

	void SetForground(S100_Foreground* value);
	S100_Foreground* GetForground();

	void SetFont(S100_Font* value);
	S100_Font* GetFont();
	void GetContents(pugi::xml_node node);
};
