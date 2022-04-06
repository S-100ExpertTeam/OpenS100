#pragma once

#include <vector>
#include <unordered_map>
#include "ExternalFile.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"


namespace Portrayal
{
	class S100_Symbols
	{
	public:
		S100_Symbols();
		virtual ~S100_Symbols();

	private:
		std::vector<ExternalFile*> s100_symbols_v;
		std::unordered_map<std::wstring, ExternalFile*> s100_symbols;

	public:
		std::vector<ExternalFile*>* GetSymbolsVector();

		void GetContents(pugi::xml_node& node);
	};
}