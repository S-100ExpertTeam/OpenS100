#pragma once
#include"stdafx.h"
#include "S100_LineStyleBase.h"
#include "S100_Pen.h"
#include "S100_Dash.h"
#include "S100_LineSymbol.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>
#include <list>

class S100_LineStyle : public S100_LineStyleBase
{
public:
	S100_LineStyle();
	virtual ~S100_LineStyle();

private:
	// attribute
	std::wstring name;
	std::wstring intervalLength;
	std::wstring width;
	std::wstring token;
	std::wstring transparency;
	std::wstring capStyle;
	std::wstring joinStyle;
	std::wstring offset;
	
	S100_Pen m_pen;
	std::list<S100_Dash> m_dash;
	std::list<S100_LineSymbol> m_symbol;

public:
	void GetContents(pugi::xml_node& node);

	void ParseValue(std::string value);

	bool IsEmpty();
	void SetEmpty();

	//set
	void SetName(std::wstring& value);
	void SetIntervalLength(std::wstring& value);
	void SetWidth(std::wstring& value);
	void SetToken(std::wstring& value);
	void SetTransparency(std::wstring& value);
	void SetCapStyle(std::wstring& value);
	void SetJoinStyle(std::wstring& value);
	void SetOffset(std::wstring& value);

	void SetPen(S100_Pen* value);
	void SetDash(S100_Dash* value);
	void SetDash(std::list<S100_Dash> value);
	void SetSymbol(S100_LineSymbol* value);
	void SetSymbol(std::list<S100_LineSymbol> value);

	//get
	std::wstring GetName();
	std::wstring GetIntervalLength();
	std::wstring GetWidth();
	std::wstring GetToken();
	std::wstring GetTransparency();
	std::wstring GetCapStyle();
	std::wstring GetJoinStyle();
	std::wstring GetOffset();
	
	S100_Pen* GetPen();
	S100_Dash* GetDash(int index);
	std::list<S100_Dash> GetDashs();
	S100_LineSymbol* GetSymbol(int index);
	std::list<S100_LineSymbol> GetSymbols();
};