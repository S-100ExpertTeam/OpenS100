#pragma once

#include "SVGReader.h"
#include <unordered_map>

class S100SymbolManager
{
public:
	S100SymbolManager();
	virtual ~S100SymbolManager();

public:
	std::unordered_map<std::string, SVGReader> svgSymbols;

public:
	// SVG folder path. (ex : C:\\Symbols\\*.svg)
	bool Open(const std::string& _path);
	bool Open(const std::wstring& _path);

	// Open single svg
	bool Add(const std::string& path);
	bool Add(const std::wstring& path);
	void CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory);
	SVGReader* GetSVG(const std::string& _name);
	SVGReader* GetSVG(const std::wstring& _name);
};