#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_SRGB
{
public:
	S100_SRGB();
	virtual ~S100_SRGB();
	
private:
	int red = 0;
	int green = 0;
	int blue = 0;

public:
	void GetContents(pugi::xml_node Node);

	void SetRed(int value);
	int GetRed();

	void SetGreen(int value);
	int GetGreen();

	void SetBlue(int value);
	int GetBlue();
};