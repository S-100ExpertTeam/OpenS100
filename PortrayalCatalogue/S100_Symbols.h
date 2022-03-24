#pragma once

#include <vector>
#include <unordered_map>

#include "..\\extlibs\pugixml\include\pugixml.hpp"

class S100_Symbol;

class S100_Symbols
{
public:
	S100_Symbols();
	virtual ~S100_Symbols();

private:
	std::vector<S100_Symbol*> s100_symbols_vec;
	std::unordered_map<std::wstring, S100_Symbol*> s100_symbols;

public:
	void GetContents(pugi::xml_node& node);
};