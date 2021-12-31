#pragma once

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <unordered_map>
#include <string>

class S100_ColorProfile;
class S100_ColorProfiles
{
public:
	S100_ColorProfiles();
	virtual ~S100_ColorProfiles();

private:
	// key : id
	std::unordered_map<std::wstring, S100_ColorProfile*> colorProfileFiles;

public:
	//Read ColorProfile
	bool ReadColorProfiles(std::wstring path);

	void GetContents(pugi::xml_node& node);

	S100_ColorProfile* GetColorProfile();
	std::unordered_map<std::wstring, S100_ColorProfile*> GetColorProfiles();

	void SetColorProfile(std::wstring key, S100_ColorProfile* value);
}; 