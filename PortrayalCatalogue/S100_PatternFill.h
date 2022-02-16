#pragma once
#include "S100_AreaFillBase.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class S100_PatternFill : public S100_AreaFillBase
{
public:
	S100_PatternFill();
	virtual ~S100_PatternFill();

private:
	std::wstring areaCRS;

public:
	void GetContents(pugi::xml_node node);

	void SetAreaCRS(std::wstring& value);
	std::wstring GetAreaCRS();
};