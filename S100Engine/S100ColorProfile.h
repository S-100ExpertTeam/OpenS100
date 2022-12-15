#pragma once
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
	bool OpenByPugi(char* filePath);

	// Current Palette returns the color corresponding to token.
	D2D1_COLOR_F GetColor(std::wstring& token);

	// Save palette pointer to current palette according to palette name
	void ChangePalette(std::wstring paletteName);

private:
	// not use fuction
	D2D1_COLOR_F GetColor(std::wstring _paletteName, std::wstring _token);

	void extractionColors(pugi::xml_node node);
	void extractionPalette(pugi::xml_node node);

	// Divide strings (separate strings using char characters)
	std::vector<CString> Split(CString _data, char parsingChar);

	// Divide strings (separate strings by specifying the number of characters)
	std::vector<CString> SplitBySpace(CString _data, int _space);

	// Hex to decimal.
	int Conert16to10(char* ch);

	//CString -> char* (delete. Please enter it!)
	//char* ConvertCstringToChar(CString inCstring);

	//char* -> CString
	CString ConvertCharToCString(char* inChar);

	//char*-> wstring
	std::wstring ConvertCtoWS(char* c);

	//wchar_t to char
	char* ConvertWCtoC(wchar_t* str);
};