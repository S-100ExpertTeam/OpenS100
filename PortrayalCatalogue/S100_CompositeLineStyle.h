#pragma once
#include "S100_LineStyleBase.h"
#include "S100_LineStyleReference.h"
#include "S100_LineStyle.h"

#include <pugixml.hpp>

#include <vector>

class S100_CompositeLineStyle : public S100_LineStyleBase
{
public:
	S100_CompositeLineStyle();
	virtual ~S100_CompositeLineStyle();

private:
	std::vector<S100_LineStyleBase*> lineStyles;

public:
	void GetContents(pugi::xml_node& node);

	void SetLineStyle(S100_LineStyleBase* value);
	void SetLineStyles(std::vector<S100_LineStyleBase*> value);

	S100_LineStyleBase* GetLineStyle(int index);
	std::vector<S100_LineStyleBase*> GetLineStyles();
};