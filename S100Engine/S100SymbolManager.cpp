#include "stdafx.h"
#include "S100SymbolManager.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

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

bool S100SymbolManager::Open(std::wstring _path)
{
	CFileFind m_fileFinder;

	BOOL bWorking = FALSE;
	
	bWorking = m_fileFinder.FindFile(_path.c_str());
	
	while (bWorking)
	{
		bWorking = m_fileFinder.FindNextFile();

		auto cFilePath = m_fileFinder.GetFilePath();
		auto wFilePath = std::wstring(cFilePath);
		auto filePath = LibMFCUtil::ConvertWCtoC((wchar_t*)wFilePath.c_str());
		SVGReader svg;
		svg.OpenByPugi(filePath);

		delete[] filePath; 

		svgSymbols.insert({ svg.name, svg });
	}
	return true;
}

SVGReader* S100SymbolManager::GetSVG(std::wstring _name)
{
	auto svgSymbol = svgSymbols.find(_name);

	if (svgSymbol != svgSymbols.end())
	{
		return &svgSymbol->second;
	}

	return nullptr;
}

void S100SymbolManager::CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory)
{
	for (auto i = svgSymbols.begin(); i != svgSymbols.end(); i++)
	{
		i->second.CreateSVGGeometry(m_pDirect2dFactory);
	}
}