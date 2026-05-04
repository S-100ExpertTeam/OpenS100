#pragma once
#include "stdafx.h"
#include "S100_LineStyleBase.h"
#include "S100_Pen.h"
#include "S100_Dash.h"
#include "S100_LineSymbol.h"

#include <pugixml.hpp>

#include <string>
#include <list>

class S100_LineStyle : public S100_LineStyleBase
{
public:
	S100_LineStyle();
	virtual ~S100_LineStyle();

private:
	std::string name;
	std::string intervalLength;
	std::string width;
	std::string token;
	std::string transparency;
	std::string capStyle;
	std::string joinStyle;
	std::string offset;

	S100_Pen m_pen;
	std::list<S100_Dash> m_dash;
	std::list<S100_LineSymbol> m_symbol;

public:
	void GetContents(pugi::xml_node& node);

	void ParseValue(std::string value);
	void ParseValue(std::string_view value);

	bool IsEmpty();
	void SetEmpty();

	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	void SetIntervalLength(const std::string& value);
	void SetIntervalLength(const std::wstring& value);
	void SetWidth(const std::string& value);
	void SetWidth(const std::wstring& value);
	void SetToken(const std::string& value);
	void SetToken(const std::wstring& value);
	void SetTransparency(const std::string& value);
	void SetTransparency(const std::wstring& value);
	void SetCapStyle(const std::string& value);
	void SetCapStyle(const std::wstring& value);
	void SetJoinStyle(const std::string& value);
	void SetJoinStyle(const std::wstring& value);
	void SetOffset(const std::string& value);
	void SetOffset(const std::wstring& value);

	void SetPen(S100_Pen* value);
	void SetDash(S100_Dash* value);
	void SetDash(std::list<S100_Dash> value);
	void SetSymbol(S100_LineSymbol* value);
	void SetSymbol(std::list<S100_LineSymbol> value);

	std::string  GetName();
	std::wstring GetNameW();
	std::string  GetIntervalLength();
	std::wstring GetIntervalLengthW();
	std::string  GetWidth();
	std::wstring GetWidthW();
	std::string  GetToken();
	std::wstring GetTokenW();
	std::string  GetTransparency();
	std::wstring GetTransparencyW();
	std::string  GetCapStyle();
	std::wstring GetCapStyleW();
	std::string  GetJoinStyle();
	std::wstring GetJoinStyleW();
	std::string  GetOffset();
	std::wstring GetOffsetW();

	S100_Pen* GetPen();
	S100_Dash* GetDash(int index);
	std::list<S100_Dash> GetDashs();
	S100_LineSymbol* GetSymbol(int index);
	std::list<S100_LineSymbol> GetSymbols();
};
