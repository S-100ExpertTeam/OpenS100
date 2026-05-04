#pragma once

#include "S100_Description.h"

#include <unordered_map>
#include <string>

class S100_Color;
class S100_Palette;

class S100_ColorProfile
{
public:
	S100_ColorProfile();
	virtual ~S100_ColorProfile();

private:
	std::unordered_map<std::string, S100_Color*> colors;
	std::vector<S100_Color*> colors_v;

	std::unordered_map<std::string, S100_Palette*> palettes;
	std::vector<S100_Palette*> palettes_v;

public:
	bool ReadFile(const std::string& path);
	bool ReadFile(const std::wstring& path);

	void GetContents(pugi::xml_node node);

	D2D1_COLOR_F GetD2Color(const std::string& paletteName, const std::string& token);
	D2D1_COLOR_F GetD2Color(const std::wstring& paletteName, const std::wstring& token);
	COLORREF GetRGBRef(const std::string& paletteName, const std::string& token);
	COLORREF GetRGBRef(const std::wstring& paletteName, const std::wstring& token);

	void Setcolors(const std::string& key, S100_Color* value);
	void Setcolors(const std::wstring& key, S100_Color* value);
	std::unordered_map<std::string, S100_Color*> Getcolors();

	void Setpaletters(const std::string& key, S100_Palette* value);
	void Setpaletters(const std::wstring& key, S100_Palette* value);
	std::unordered_map<std::string, S100_Palette*> Getpalettes();
};
