#pragma once
#include "S100_Color.h"

class S100_OverrideColor
{
public:
	S100_OverrideColor();
	virtual ~S100_OverrideColor();

private:
	// element
	S100_Color overrideColor;
	S100_Color color;

public:
	void GetContents(pugi::xml_node node);

	void SetOverrideColor(S100_Color* value);
	S100_Color* GetOverrideColor();

	void SetColor(S100_Color* value);
	S100_Color* GetColor();
};