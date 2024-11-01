#pragma once

#include "SENC_PatternFill.h"
#include "SENC_Symbol.h"
#include "SENC_VectorPoint.h"

#include <memory>

class AreaFill;

class SENC_SymbolFill : public SENC_PatternFill
{
public:
	SENC_SymbolFill();
	~SENC_SymbolFill();

public:
	SENC_Symbol* symbol;

	SENC_VectorPoint v1;
	SENC_VectorPoint v2;

	bool clipSymbols;

public:
	std::unique_ptr<AreaFill> as_AreaFill();
};

