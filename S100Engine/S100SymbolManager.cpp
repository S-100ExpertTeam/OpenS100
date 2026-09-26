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

bool S100SymbolManager::Open(const std::string& _path)
{
	return Open(LibMFCUtil::StringToWString(_path));
}

bool S100SymbolManager::Open(const std::wstring& _path)
{
	CFileFind m_fileFinder;

	BOOL bWorking = m_fileFinder.FindFile(_path.c_str());

	while (bWorking)
	{
		bWorking = m_fileFinder.FindNextFile();

		auto cFilePath = m_fileFinder.GetFilePath();
		auto charPath = LibMFCUtil::ConvertWCtoC(cFilePath.GetBuffer());
		Add(std::string(charPath));
		delete[] charPath;
	}
	return true;
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
