#pragma once

#include "SENC_LineStyleBase.h"

#include <vector>
#include <string>

class S100_CompositeLineStyle;
class SENC_LineStyle;
class PortrayalCatalogue;
class SENC_CompositeLineStyle : public SENC_LineStyleBase
{
public:
	SENC_CompositeLineStyle();
	~SENC_CompositeLineStyle();

public:
	std::vector<SENC_LineStyleBase*> lineStyles;

public:
	void GetStyleFromS100(
		S100_CompositeLineStyle* lineStyle, 
		PortrayalCatalogue *pc,
		std::vector<SENC_LineStyle*> &lineStyles);


public: // override from SENC_LineStyleBase
	void Accept(SENC_LineStyleVisitor& visitor) override { visitor.Visit(*this); }
};