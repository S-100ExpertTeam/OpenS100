#pragma once

#include <pugixml.hpp>

#include <string>

class S100_Color
{
public:
	S100_Color();
	virtual ~S100_Color();

private:
	std::string token;
	std::string name;
	std::string transparency;
	std::string description;

public:
	void GetContents(pugi::xml_node& node);

	void SetToken(const std::string& value);
	void SetToken(const std::wstring& value);
	std::string  GetToken();
	std::wstring GetTokenW();

	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	std::string  GetName();
	std::wstring GetNameW();

	void SetDescription(const std::string& value);
	void SetDescription(const std::wstring& value);
	std::string  GetDescription();
	std::wstring GetDescriptionW();

	void SetTransparency(const std::string& value);
	void SetTransparency(const std::wstring& value);
	std::string  GetTransparency();
	std::wstring GetTransparencyW();
};
