#include "stdafx.h"
#include "SENC_SymbolFill.h"

#include "..\\S100Engine\\AreaFill.h"

SENC_SymbolFill::SENC_SymbolFill()
{
	type = 4;
	symbol = NULL;
}


SENC_SymbolFill::~SENC_SymbolFill()
{
	if (symbol) delete symbol;
}

std::unique_ptr<AreaFill> SENC_SymbolFill::as_AreaFill()
{
	auto areaFill = std::make_unique<AreaFill>();
	areaFill->v1_x = v1.x;
	areaFill->v1_y = v1.y;
	areaFill->v2_x = v2.x;
	areaFill->v2_y = v2.y;
	areaFill->_symbolReference = symbol->reference;
	areaFill->offset_x = symbol->offset.x;
	areaFill->offset_y = symbol->offset.y;
	return areaFill;
}