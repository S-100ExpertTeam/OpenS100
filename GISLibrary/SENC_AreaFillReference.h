#pragma once
#include "SENC_AreaFillBase.h"

class SENC_AreaFillReference :
	public SENC_AreaFillBase
{
public:
	SENC_AreaFillReference();
	~SENC_AreaFillReference();

public:
	int reference;
};