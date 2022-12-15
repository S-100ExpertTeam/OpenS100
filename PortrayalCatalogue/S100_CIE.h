#pragma once

#include <pugixml.hpp>

#include <string>

class S100_CIE
{
public:
	S100_CIE();
	virtual ~S100_CIE();

private:
	float x = 0.0f;
	float y = 0.0f;
	float L = 0.0f;

public:
	void GetContents(pugi::xml_node Node);
	
	void SetX(float value);
	float GetX();

	void SetY(float value);
	float GetY();

	void SetL(float value);
	float GetL();
};