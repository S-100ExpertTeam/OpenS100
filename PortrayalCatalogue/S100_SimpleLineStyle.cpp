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

void S100_SimpleLineStyle::SetCapStyle(std::wstring& value)
{
	capStyle = value;
}

void S100_SimpleLineStyle::SetJoinStyle(std::wstring& value)
{
	joinStyle = value;
}

void S100_SimpleLineStyle::SetOffset(std::wstring& value)
{
	offset = value;
}

void S100_SimpleLineStyle::SetPen(S100_Pen* value) 
{
	pen = value;
}

void S100_SimpleLineStyle::SetDash(S100_Dash* value) 
{
	dash = value;
}


std::wstring S100_SimpleLineStyle::GetCapStyle()
{
	return capStyle;
}

std::wstring S100_SimpleLineStyle::GetJoinStyle() 
{
	return joinStyle;
}

std::wstring S100_SimpleLineStyle::GetOffset() 
{
	return offset;
}

S100_Pen* S100_SimpleLineStyle::GetPen() 
{
	return pen;
}

S100_Dash* S100_SimpleLineStyle::GetDash() 
{
	return dash;
}