#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_VectorPoint
{
public:
	S100_VectorPoint();
	virtual ~S100_VectorPoint();

private:
	std::wstring x;
	std::wstring y;

public:
	void GetContents(pugi::xml_node& node);

	void SetX(std::wstring& value);
	void SetY(std::wstring& value);

	std::wstring GetX();
	std::wstring GetY();
};