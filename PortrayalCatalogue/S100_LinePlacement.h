#pragma once

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_LinePlacement
{
public:
	S100_LinePlacement();
	virtual ~S100_LinePlacement();

private:
	std::wstring offset;
	std::wstring placementMode;

public:
	void GetContents(pugi::xml_node& node);

	void SetOffset(std::wstring& value);
	std::wstring GetOffset();

	void SetPlacementMode(std::wstring& value);
	std::wstring GetPlacementMode();
};