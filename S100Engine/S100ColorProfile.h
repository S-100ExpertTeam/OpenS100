#pragma once
#include "RenderPrelude.h"
#include "Palette.h"
#include "Colors.h"

#include <pugixml.hpp>

#include <vector>

class S100ColorProfile
{
public:
	S100ColorProfile();
	virtual~S100ColorProfile();

public:
	std::vector<Palette> palette;
	libS100Engine::Colors colors;
	Palette* currentPalette = nullptr;

public:
	bool OpenByPugi(const char* filePath);

	// Current Palette returns the color corresponding to token.
	D2D1_COLOR_F GetColor(const std::string& token);
	D2D1_COLOR_F GetColor(const std::wstring& token);
	D2D1_COLOR_F GetColor(const std::string& paletteName, const std::string& token);
	D2D1_COLOR_F GetColor(const std::wstring& paletteName, const std::wstring& token);

	// Save palette pointer to current palette according to palette name
	void ChangePalette(const std::string& paletteName);
	void ChangePalette(const std::wstring& paletteName);

private:
	void extractionColors(pugi::xml_node node);
	void extractionPalette(pugi::xml_node node);



	// Hex to decimal.
	int Conert16to10(char* ch);



	//char*-> wstring
	std::wstring ConvertCtoWS(char* c);

	//wchar_t to char
	char* ConvertWCtoC(wchar_t* str);
};