#pragma once
#include "S100_AreaFillBase.h"

#include <pugixml.hpp>

class S100_PatternFill : public S100_AreaFillBase
{
public:
	S100_PatternFill();
	virtual ~S100_PatternFill();

private:
	std::string areaCRS;

public:
	void GetContents(pugi::xml_node node);

	void SetAreaCRS(const std::string& value);
	void SetAreaCRS(const std::wstring& value);
	std::string  GetAreaCRS();
	std::wstring GetAreaCRSW();
};
