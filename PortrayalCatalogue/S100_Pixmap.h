#pragma once
#include "S100_Color.h"
#include "S100_OverrideColor.h"

#include <string>
class S100_Pixmap
{
public:
	S100_Pixmap();
	virtual ~S100_Pixmap();

private:
	// attribute 
	std::wstring reference;
	// element
	S100_Color overrideAll;
	S100_OverrideColor overrideColor;

public:
	void GetContents(pugi::xml_node node);

	void SetReference(std::wstring& value);
	std::wstring GetReference();

	void SetOverrideAll(S100_Color* value);
	S100_Color* GetOverrideAll();

	void SetOverrideColor(S100_OverrideColor* value);
	S100_OverrideColor* GetOverrideColor();
};