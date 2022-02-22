#pragma once
#include <string>
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class S100_Foreground 
{
public:
	S100_Foreground();
	virtual ~S100_Foreground();
private:
	std::wstring token;
	std::wstring transparency;

public:
	void GetContents(pugi::xml_node node);

	void SetToken(std::wstring& value);
	std::wstring GetToken();

	void SetTransparency(std::wstring& value);
	std::wstring GetTransparency();
};