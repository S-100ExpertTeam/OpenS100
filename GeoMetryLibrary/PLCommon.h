#pragma once
enum eColor
{
	eColor_Acronym,
	eColor_Name,
	eColor_R,
	eColor_G,
	eColor_B
};

enum predefined_line_style
{
	SOLD = 1,
	DASH,
	DOTT
};

enum eSYParameters
{
	SY_SYNAME = 1,
	SY_ROTATION
};

enum eTEParameters
{
	TE_STRING = 1,
	TE_ACODE,
	TE_HJUST,
	TE_VJUST,
	TE_SPACE,
	TE_CHARS,
	TE_XOFFS,
	TE_YOFFS,
	TE_COLOUR,
	TE_DISPLAY
};

enum eTXParameters
{
	TX_ACODE = 1,
	TX_HJUST,
	TX_VJUST,
	TX_SPACE,
	TX_CHARS,
	TX_XOFFS,
	TX_YOFFS,
	TX_COLOUR,
	TX_DISPLAY
};

enum eLSParameters
{
	LS_PSTYLE = 1,
	LS_WIDTH,
	LS_COLOUR
};

enum eACParameters
{
	AC_COLOUR = 1,
	AC_TRANSPARENCY
};

enum eAPParameters
{
	AP_PATNAME = 1,
	AP_ROTATION
};

struct CHARS
{
	int STYLE;
	int WEIGHT;
	int WIDTH;
	int BODYSIZE;
};

struct COLOUR
{
	CString Acronym;
	CString Name;
	int R;
	int G;
	int B;
};

struct Field2
{
	int	    Code;
	CString	number;
};

/*
The smaller the value, the bigger the symbol.
It is used in Symbol::MakeVector().
*/
#define MINIMIZING_VALUE 30

struct PointD
{
	double x = 0;
	double y = 0;
	double z = 0;
};

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#define VECTOR_CIRCLE_MODE 0x0001
#define VECTOR_POLYGON_MODE 0x0010
#define VECTOR_LINE_MODE 0x0100
#define VECTOR_POINT_MODE 0x1000

struct SymbolColorRef
{
	char pen_name;
	char color_name[6];
	COLORREF color;
};

struct SymbolColor 
{
	char name[6];
	COLORREF rgb;
};