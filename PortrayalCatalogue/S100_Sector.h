#pragma once
#include <pugixml.hpp>

class S100_Sector
{
public:
	S100_Sector();
	virtual ~S100_Sector();

private:
	std::wstring startAngle;
	std::wstring angularDistance;

public:
	void GetContents(pugi::xml_node node);

	void SetStartAngle(std::wstring& value);
	void SetStartAngle(std::string& value);
	void SetAnglearDistance(std::wstring& value);
	void SetAnglearDistance(std::string& value);

	std::wstring GetStartAngle();
	std::wstring GetAnglearDistance();
};