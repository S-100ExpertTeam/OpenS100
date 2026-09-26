#pragma once
#include <pugixml.hpp>
#include <string>

class S100_Sector
{
public:
	S100_Sector();
	virtual ~S100_Sector();

private:
	std::string startAngle;
	std::string angularDistance;

public:
	void GetContents(pugi::xml_node node);

	void SetStartAngle(const std::string& value);
	void SetStartAngle(const std::wstring& value);
	std::string  GetStartAngle();
	std::wstring GetStartAngleW();

	void SetAnglearDistance(const std::string& value);
	void SetAnglearDistance(const std::wstring& value);
	std::string  GetAnglearDistance();
	std::wstring GetAnglearDistanceW();
};
