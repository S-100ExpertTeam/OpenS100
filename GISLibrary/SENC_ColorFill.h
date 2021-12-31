#pragma once
#include "SENC_AreaFillBase.h"

class SENC_Color;
class PortrayalCatalogue;
class SENC_ColorFill : public SENC_AreaFillBase
{
public:
	SENC_ColorFill();
	~SENC_ColorFill();

public:
	SENC_Color *color = nullptr;

public:
	void ChangePallete(PortrayalCatalogue *pc);
};