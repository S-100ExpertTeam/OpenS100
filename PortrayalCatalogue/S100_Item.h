#pragma once
#include "S100_CIE.h"
#include "S100_SRGB.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_Item
{
public:
	S100_Item();
	virtual ~S100_Item();

private:
	std::wstring token;
	S100_CIE scie;
	S100_SRGB srgb;

public:
	void GetContents(pugi::xml_node Node);

	void SetToken(std::wstring& value);
	void SetSCIE(S100_CIE* value);
	void SetSRGB(S100_SRGB* value);

	std::wstring GetToken();
	S100_CIE* GetSCIE();
	S100_SRGB* GetSRGB();
};