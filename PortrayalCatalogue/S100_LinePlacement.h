#pragma once

#include <pugixml.hpp>

#include <string>

class S100_LinePlacement
{
public:
	S100_LinePlacement();
	virtual ~S100_LinePlacement();

private:
	std::string offset;
	std::string placementMode;

public:
	void GetContents(pugi::xml_node& node);

	void SetOffset(const std::string& value);
	void SetOffset(const std::wstring& value);
	std::string  GetOffset();
	std::wstring GetOffsetW();

	void SetPlacementMode(const std::string& value);
	void SetPlacementMode(const std::wstring& value);
	std::string  GetPlacementMode();
	std::wstring GetPlacementModeW();
};
