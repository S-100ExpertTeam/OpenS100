#pragma once
#include <pugixml.hpp>

#include <string>

class S100_Font
{
public:
	S100_Font();
	virtual ~S100_Font();

private:
	std::string serifs;
	std::string weight;
	std::string slant;
	std::string proportion;

public:
	void GetContents(pugi::xml_node node);

	void SetSerifs(const std::string& value);
	void SetSerifs(const std::wstring& value);
	std::string  GetSerifs();
	std::wstring GetSerifsW();

	void SetWeight(const std::string& value);
	void SetWeight(const std::wstring& value);
	std::string  GetWeight();
	std::wstring GetWeightW();

	void SetSlant(const std::string& value);
	void SetSlant(const std::wstring& value);
	std::string  GetSlant();
	std::wstring GetSlantW();

	void SetProportion(const std::string& value);
	void SetProportion(const std::wstring& value);
	std::string  GetProportion();
	std::wstring GetProportionW();

	bool isUpright();
};
