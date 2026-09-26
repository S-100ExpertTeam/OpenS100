#include "stdafx.h"
#include "S100_LineStyle.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

S100_LineStyle::S100_LineStyle()
{
	capStyle = "Butt";
	joinStyle = "Miter";
	offset = "0.0";
	SetType(1);
}

S100_LineStyle::~S100_LineStyle()
{

}

void S100_LineStyle::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for each (pugi::xml_attribute attri in node.attributes())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "capStyle"))
		{
			capStyle = attri.value();
		}
		else if (!strcmp(attriName, "joinStyle"))
		{
			joinStyle = attri.value();
		}
		else if (!strcmp(attriName, "offset"))
		{
			offset = attri.value();
		}
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "intervalLength"))
		{
			intervalLength = instruction.child_value();
		}
		else if (!strcmp(instructionName, "pen"))
		{
			m_pen.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "dash"))
		{
			S100_Dash dash;
			dash.GetContents(instruction);
			m_dash.push_back(dash);
		}
		else if (!strcmp(instructionName, "symbol"))
		{
			S100_LineSymbol symbol;
			symbol.GetContents(instruction);
			m_symbol.push_back(symbol);
		}
	}
}

void S100_LineStyle::ParseValue(std::string value)
{
	SetEmpty();

	std::vector<std::string> v_splited = LatLonUtility::Split(value, ",");
	if (v_splited.size() >= 4)
	{
		name         = v_splited[0];
		intervalLength = v_splited[1];
		width        = v_splited[2];
		token        = v_splited[3];

		if (v_splited.size() >= 5) { transparency = v_splited[4]; }
		if (v_splited.size() >= 6) { capStyle = v_splited[5]; }
		if (v_splited.size() >= 7) { joinStyle = v_splited[6]; }
		if (v_splited.size() >= 8) { offset = v_splited[7]; }

		m_pen.SetWidth(width);
		m_pen.GetColor().SetToken(token);
		try { m_pen.GetColor().SetTransparency(std::stof(transparency)); }
		catch (...) { m_pen.GetColor().SetTransparency(0.0f); }
	}
}

void S100_LineStyle::ParseValue(std::string_view value)
{
	SetEmpty();

	std::vector<std::string_view> v_splited;
	LatLonUtility::Split(value, ",", v_splited);
	if (v_splited.size() >= 4)
	{
		name           = std::string(v_splited[0]);
		intervalLength = std::string(v_splited[1]);
		width          = std::string(v_splited[2]);
		token          = std::string(v_splited[3]);

		if (v_splited.size() >= 5) { transparency = std::string(v_splited[4]); }
		if (v_splited.size() >= 6) { capStyle = std::string(v_splited[5]); }
		if (v_splited.size() >= 7) { joinStyle = std::string(v_splited[6]); }
		if (v_splited.size() >= 8) { offset = std::string(v_splited[7]); }

		m_pen.SetWidth(width);
		m_pen.GetColor().SetToken(token);
		try { m_pen.GetColor().SetTransparency(std::stof(transparency)); }
		catch (...) { m_pen.GetColor().SetTransparency(0.0f); }
	}
}

bool S100_LineStyle::IsEmpty()
{
	return width.empty();
}

void S100_LineStyle::SetEmpty()
{
	name = "";
	intervalLength = "";
	width = "";
	token = "";
	transparency = "0";
	capStyle = "Butt";
	joinStyle = "Miter";
	offset = "0.0";

	m_dash.clear();
	m_symbol.clear();
}

void S100_LineStyle::SetName(const std::string& value)  { name = value; }
void S100_LineStyle::SetName(const std::wstring& value) { name = toUtf8(value); }
void S100_LineStyle::SetIntervalLength(const std::string& value)  { intervalLength = value; }
void S100_LineStyle::SetIntervalLength(const std::wstring& value) { intervalLength = toUtf8(value); }
void S100_LineStyle::SetWidth(const std::string& value)  { width = value; }
void S100_LineStyle::SetWidth(const std::wstring& value) { width = toUtf8(value); }
void S100_LineStyle::SetToken(const std::string& value)  { token = value; }
void S100_LineStyle::SetToken(const std::wstring& value) { token = toUtf8(value); }
void S100_LineStyle::SetTransparency(const std::string& value)  { transparency = value; }
void S100_LineStyle::SetTransparency(const std::wstring& value) { transparency = toUtf8(value); }
void S100_LineStyle::SetCapStyle(const std::string& value)  { capStyle = value; }
void S100_LineStyle::SetCapStyle(const std::wstring& value) { capStyle = toUtf8(value); }
void S100_LineStyle::SetJoinStyle(const std::string& value)  { joinStyle = value; }
void S100_LineStyle::SetJoinStyle(const std::wstring& value) { joinStyle = toUtf8(value); }
void S100_LineStyle::SetOffset(const std::string& value)  { offset = value; }
void S100_LineStyle::SetOffset(const std::wstring& value) { offset = toUtf8(value); }

void S100_LineStyle::SetPen(S100_Pen* value)  { m_pen = *value; }

void S100_LineStyle::SetDash(S100_Dash* value)        { m_dash.push_back(*value); }
void S100_LineStyle::SetDash(std::list<S100_Dash> value) { m_dash = value; }

void S100_LineStyle::SetSymbol(S100_LineSymbol* value)           { m_symbol.push_back(*value); }
void S100_LineStyle::SetSymbol(std::list<S100_LineSymbol> value) { m_symbol = value; }

std::string  S100_LineStyle::GetName()           { return name; }
std::wstring S100_LineStyle::GetNameW()          { return toWide(name); }
std::string  S100_LineStyle::GetIntervalLength() { return intervalLength; }
std::wstring S100_LineStyle::GetIntervalLengthW(){ return toWide(intervalLength); }
std::string  S100_LineStyle::GetWidth()          { return width; }
std::wstring S100_LineStyle::GetWidthW()         { return toWide(width); }
std::string  S100_LineStyle::GetToken()          { return token; }
std::wstring S100_LineStyle::GetTokenW()         { return toWide(token); }
std::string  S100_LineStyle::GetTransparency()   { return transparency; }
std::wstring S100_LineStyle::GetTransparencyW()  { return toWide(transparency); }
std::string  S100_LineStyle::GetCapStyle()       { return capStyle; }
std::wstring S100_LineStyle::GetCapStyleW()      { return toWide(capStyle); }
std::string  S100_LineStyle::GetJoinStyle()      { return joinStyle; }
std::wstring S100_LineStyle::GetJoinStyleW()     { return toWide(joinStyle); }
std::string  S100_LineStyle::GetOffset()         { return offset; }
std::wstring S100_LineStyle::GetOffsetW()        { return toWide(offset); }

S100_Pen* S100_LineStyle::GetPen() { return &m_pen; }

S100_Dash* S100_LineStyle::GetDash(int index)
{
	auto it = m_dash.begin();
	advance(it, index);
	return &(*it);
}

std::list<S100_Dash> S100_LineStyle::GetDashs() { return m_dash; }

S100_LineSymbol* S100_LineStyle::GetSymbol(int index)
{
	auto it = m_symbol.begin();
	advance(it, index);
	return &(*it);
}

std::list<S100_LineSymbol> S100_LineStyle::GetSymbols() { return m_symbol; }
