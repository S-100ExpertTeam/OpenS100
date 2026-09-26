#pragma once

#include <string>
#include "Enum_ColorTable.h"
#include "Enum_DisplayModeTable.h"

class ENCCommon
{
public:
	static bool	TEXTOUT;

	static bool	FULL_SECTORS;

	// if true then apply scale min 
	static bool APPLY_SCALE_MIN;

	static double SAFETY_CONTOUR;			// unit : meter (optional)
	static double SHALLOW_CONTOUR;			// unit : meter [IMP PS [3]]
	static double DEEP_CONTOUR;				// unit : meter (optional)

	static int DISPLAY_FONT_SIZE;

	static std::string DISPLAY_FONT_NAME;
	static std::wstring getDisplayFontNameW();

	static GeoMetryLibrary::DisplayModeTable DISPLAY_MODE;
	static GeoMetryLibrary::ColorTable m_eColorTable;

	static float DISPLAY_SYMBOL_SCALE;

	// true:  center Point(fluid)
	// false: All AREA center point(fixed)
	static bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

	static bool Show_INFORM01; //inform01 show / hide

public:
	static bool Save(std::string filePath);
	static bool Save(std::wstring filePath);
	static bool Open(std::string filePath);
	static bool Open(std::wstring filePath);
};
