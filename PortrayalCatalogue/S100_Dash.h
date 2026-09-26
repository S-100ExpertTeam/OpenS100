#pragma once

#include <pugixml.hpp>

#include <string>

class S100_Dash
{
public:
	S100_Dash();
	virtual ~S100_Dash();

private:
	std::string start;
	std::string length;

public:
	void SetStart(const std::string& value);
	void SetStart(const std::wstring& value);
	std::string  GetStart();
	std::wstring GetStartW();

	void SetLength(const std::string& value);
	void SetLength(const std::wstring& value);
	std::string  GetLength();
	std::wstring GetLengthW();

	void GetContents(pugi::xml_node& node);

	void SetFromStartLength(std::string& value);

	bool IsEmpty();
	void SetEmpty();

	void ParseValue(std::string value);
	void ParseValue(std::string_view value);
};
