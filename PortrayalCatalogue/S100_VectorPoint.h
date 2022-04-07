#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_VectorPoint
{
public:
	S100_VectorPoint();
	virtual ~S100_VectorPoint();
private:
	double x = 0;
	double y = 0;
public:
	void GetContents(pugi::xml_node& node);
	void SetX(std::wstring& value);
	void SetY(std::wstring& value);
	void SetX(double value);
	void SetY(double value);
	double GetX();
	double GetY();
};