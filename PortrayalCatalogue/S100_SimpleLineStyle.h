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
	std::string capStyle;
	std::string joinStyle;
	std::string offset;
	S100_Pen *pen;
	S100_Dash *dash;

public:
	void SetCapStyle(const std::string& value);
	void SetCapStyle(const std::wstring& value);
	void SetJoinStyle(const std::string& value);
	void SetJoinStyle(const std::wstring& value);
	void SetOffset(const std::string& value);
	void SetOffset(const std::wstring& value);
	void SetPen(S100_Pen* value);
	void SetDash(S100_Dash* value);

	std::string  GetCapStyle();
	std::wstring GetCapStyleW();
	std::string  GetJoinStyle();
	std::wstring GetJoinStyleW();
	std::string  GetOffset();
	std::wstring GetOffsetW();
	S100_Pen* GetPen();
	S100_Dash* GetDash();
};
