#include "stdafx.h"
#include "S100_SimpleLineStyle.h"

S100_SimpleLineStyle::S100_SimpleLineStyle()
{
	pen = NULL;
	dash = NULL;
}

S100_SimpleLineStyle::~S100_SimpleLineStyle()
{
	if (pen) delete pen;
	if (dash) delete dash;
}

void S100_SimpleLineStyle::SetCapStyle(const std::string& value)  { capStyle = value; }
void S100_SimpleLineStyle::SetCapStyle(const std::wstring& value) { capStyle = toUtf8(value); }
void S100_SimpleLineStyle::SetJoinStyle(const std::string& value)  { joinStyle = value; }
void S100_SimpleLineStyle::SetJoinStyle(const std::wstring& value) { joinStyle = toUtf8(value); }
void S100_SimpleLineStyle::SetOffset(const std::string& value)  { offset = value; }
void S100_SimpleLineStyle::SetOffset(const std::wstring& value) { offset = toUtf8(value); }
void S100_SimpleLineStyle::SetPen(S100_Pen* value) { pen = value; }
void S100_SimpleLineStyle::SetDash(S100_Dash* value) { dash = value; }

std::string  S100_SimpleLineStyle::GetCapStyle()  { return capStyle; }
std::wstring S100_SimpleLineStyle::GetCapStyleW() { return toWide(capStyle); }
std::string  S100_SimpleLineStyle::GetJoinStyle()  { return joinStyle; }
std::wstring S100_SimpleLineStyle::GetJoinStyleW() { return toWide(joinStyle); }
std::string  S100_SimpleLineStyle::GetOffset()  { return offset; }
std::wstring S100_SimpleLineStyle::GetOffsetW() { return toWide(offset); }
S100_Pen*  S100_SimpleLineStyle::GetPen()  { return pen; }
S100_Dash* S100_SimpleLineStyle::GetDash() { return dash; }
