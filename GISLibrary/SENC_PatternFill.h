#pragma once
#include "SENC_AreaFillBase.h"

class SENC_PatternFill :
	public SENC_AreaFillBase
{
public:
	SENC_PatternFill();
	virtual ~SENC_PatternFill();

public:
	int areaCRSType;
};