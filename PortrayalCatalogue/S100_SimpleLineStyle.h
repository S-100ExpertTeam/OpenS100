#pragma once

#include "S100_Pen.h"
#include "S100_Dash.h"

#include <string>

class S100_SimpleLineStyle
{
public:
	S100_SimpleLineStyle();
	virtual ~S100_SimpleLineStyle();

private:
	std::wstring capStyle;
	std::wstring joinStyle;
	std::wstring offset;
	S100_Pen *pen;
	S100_Dash *dash;

public:
	void SetCapStyle(std::wstring& value);
	void SetJoinStyle(std::wstring& value);
	void SetOffset(std::wstring& value);
	void SetPen(S100_Pen* value);
	void SetDash(S100_Dash* value);

	std::wstring GetCapStyle();
	std::wstring GetJoinStyle();
	std::wstring GetOffset();
	S100_Pen* GetPen();
	S100_Dash* GetDash();
};