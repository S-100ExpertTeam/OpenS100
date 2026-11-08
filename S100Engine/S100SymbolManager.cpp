#include "stdafx.h"
#include "S100SymbolManager.h"

#include <pugixml.hpp>

S100SymbolManager::S100SymbolManager()
{

}

S100SymbolManager::~S100SymbolManager()
{
	for (auto i = svgSymbols.begin(); i != svgSymbols.end(); i++)
	{
		i->second.Close();
	}

	svgSymbols.clear();
}

bool S100SymbolManager::Add(const std::string& path)
{
	SVGReader svg;
	svg.OpenByPugi(const_cast<char*>(path.c_str()));

	if (GetSVG(svg.name))
	{
		return false;
	}

	auto name = svg.name;
	svgSymbols.emplace(name, std::move(svg));
	return true;
}

bool S100SymbolManager::Add(const std::wstring& path)
{
	return Add(LibMFCUtil::WStringToString(path));
}

SVGReader* S100SymbolManager::GetSVG(const std::string& _name)
{
	auto svgSymbol = svgSymbols.find(_name);
	if (svgSymbol != svgSymbols.end())
	{
		return &svgSymbol->second;
	}
	return nullptr;
}

SVGReader* S100SymbolManager::GetSVG(const std::wstring& _name)
{
	return GetSVG(LibMFCUtil::WStringToString(_name));
}

void S100SymbolManager::CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory)
{
	for (auto i = svgSymbols.begin(); i != svgSymbols.end(); i++)
	{
		i->second.CreateSVGGeometry(m_pDirect2dFactory);
	}
}
