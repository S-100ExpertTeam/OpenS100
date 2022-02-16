#pragma once
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class S100_Font
{
public:
	S100_Font();
	virtual ~S100_Font();

private:
	std::wstring serifs;
	std::wstring weight;
	std::wstring slant;
	std::wstring proportion;

public:
	void GetContents(pugi::xml_node node);

	void SetSerifs(std::wstring& value);
	std::wstring GetSerifs();

	void SetWeight(std::wstring& value);
	std::wstring GetWeight();

	void SetSlant(std::wstring& value);
	std::wstring GetSlant();

	void SetProportion(std::wstring& value);
	std::wstring GetProportion();
};