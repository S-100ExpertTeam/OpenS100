#pragma once
#include "S100_Symbol.h"
#include "S100_VectorPoint.h"
#include "S100_PatternFill.h"

#include <pugixml.hpp>

class S100_SymbolFill
	: public S100_PatternFill
{
public:
	S100_SymbolFill();
	virtual ~S100_SymbolFill();

private:
	// attribute
	std::string clipSymbols;
	// elements
	S100_Symbol* symbol;
	S100_VectorPoint* v1;
	S100_VectorPoint* v2;

public:
	bool ReadFileByPugiXml(std::wstring& path);
	void GetContents(pugi::xml_node& node);

	void SetClipSymbols(const std::string& value);
	void SetClipSymbols(const std::wstring& value);
	void SetSymbol(S100_Symbol* value);
	void SetV1(S100_VectorPoint* value);
	void SetV2(S100_VectorPoint* value);

	std::string  GetClipSymbols();
	std::wstring GetClipSymbolsW();
	S100_Symbol* GetSymbol();
	S100_VectorPoint* GetV1();
	S100_VectorPoint* GetV2();
};
