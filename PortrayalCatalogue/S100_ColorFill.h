#pragma once
#include "S100_AreaFillBase.h"
#include "S100_Color.h"

class S100_ColorFill : public S100_AreaFillBase
{
public:
	S100_ColorFill();
	virtual ~S100_ColorFill();

private:
	S100_Color *color;

public:
	void GetContents(pugi::xml_node node);

	void SetColor(S100_Color* value);
	S100_Color* GetColor();
};