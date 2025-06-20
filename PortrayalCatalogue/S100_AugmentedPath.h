#pragma once
#include "S100_AugmentedLineOrArea.h"
#include "S100_Path.h"

class S100_AugmentedPath :
	public S100_AugmentedLineOrArea
{
public:
	S100_AugmentedPath();
	virtual ~S100_AugmentedPath();

private:
	S100_Path* path;

public:
	void GetContents(pugi::xml_node node);

	void SetPath(S100_Path* value);
	void S100_AugmentedPath::SetPath(std::wstring& x, std::wstring& y, std::wstring& radius, std::wstring& angle, std::wstring& distance);
	S100_Path* GetPath();
};