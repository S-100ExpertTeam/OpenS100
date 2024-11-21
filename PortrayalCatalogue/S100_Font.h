#pragma once
#include <pugixml.hpp>

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
	void SetSerifs(std::string& value);
	std::wstring GetSerifs();

	void SetWeight(std::wstring& value);
	void SetWeight(std::string& value);
	std::wstring GetWeight();

	void SetSlant(std::wstring& value);
	void SetSlant(std::string& value);
	std::wstring GetSlant();

	void SetProportion(std::wstring& value);
	void SetProportion(std::string& value);
	std::wstring GetProportion();

	bool isUpright();
};