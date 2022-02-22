#pragma once
#include "S100_LineStyleBase.h"
#include "S100_LineStyleReference.h"
#include "S100_LineStyle.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <vector>

class S100_CompositeLineStyle : public S100_LineStyleBase
{
public:
	S100_CompositeLineStyle();
	virtual ~S100_CompositeLineStyle();

private:
	std::vector<S100_LineStyle*> lineStyles;

public:
	void GetContents(pugi::xml_node& node);

	void SetLineStyle(S100_LineStyle* value);
	void SetLineStyles(std::vector<S100_LineStyle*> value);

	S100_LineStyle* GetLineStyle(int index);
	std::vector<S100_LineStyle*> GetLineStyles();
};