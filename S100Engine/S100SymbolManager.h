#pragma once

#include "SVGReader.h"
#include <unordered_map>

class S100SymbolManager
{
public:
	S100SymbolManager();
	virtual ~S100SymbolManager();

public:
	std::unordered_map<std::wstring, SVGReader> svgSymbols;

public:
	// SVG folder path. (ex : C:\\Symbols\\*.svg)
	bool Open(std::wstring _path);

	// Open single svg
	bool Add(std::wstring path);
	void CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory);
	SVGReader* GetSVG(std::wstring _name);
};