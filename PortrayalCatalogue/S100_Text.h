#pragma once
#include <pugixml.hpp>

#include <string>

class S100_Text
{
public:
	S100_Text();
	virtual ~S100_Text();

private:
	std::string value;

	// Not used
	std::string useValueOf;

public:
	void GetContents(pugi::xml_node node);

	void SetValue(const std::string& value);
	void SetValue(const std::wstring& value);
	void SetUseValueOf(const std::string& value);
	void SetUseValueOf(const std::wstring& value);

	std::string  GetValue();
	std::wstring GetValueW();
	std::string  GetUseValueOf();
	std::wstring GetUseValueOfW();
};
