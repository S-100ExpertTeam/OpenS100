#pragma once

#include <pugixml.hpp>

#include <string>

class S100_Color
{
public:
	S100_Color();
	virtual ~S100_Color();

private:
	std::wstring token;
	std::wstring name;
	std::wstring transparency; 
	std::wstring description;

public:
	void GetContents(pugi::xml_node& node);

	void SetToken(std::wstring& value);
	void SetToken(std::string& value);
	std::wstring GetToken();

	void SetName(std::wstring& value);
	void SetName(std::string& value);
	std::wstring GetName();

	void SetDescription(std::wstring&  value);
	void SetDescription(std::string& value);
	std::wstring GetDescription();

	void SetTransparency(std::wstring& value);
	void SetTransparency(std::string& value);
	std::wstring GetTransparency();
};