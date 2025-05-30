#pragma once

#include <unordered_map>
#include <string>

namespace GeoMetryLibrary
{
	enum class ColorTable
	{
		Day = 0,
		Dusk,
		Night,
	};

	enum class DisplayModeTable
	{
		base = 0,
		standard = 1,
		all = 2
	};
}

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

	static std::wstring DISPLAY_FONT_NAME;

	static GeoMetryLibrary::DisplayModeTable DISPLAY_MODE;
	static GeoMetryLibrary::ColorTable m_eColorTable;

	static float DISPLAY_SYMBOL_SCALE;

	// true:  center Point(fluid)
	// false: All AREA center point(fixed)
	static bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

	static bool Show_INFORM01; //inform01 show / hide

public:
	static bool Save(std::wstring filePath);
	static bool Open(std::wstring filePath);
};