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

	enum class UserMode
	{
		User_Mode,
		Developer_Mode,
		User_Developer_Mode
	};

	enum class ReportingData
	{
		None, 
		PC,
		FC,
		DataSet
	};

	enum class UnitDistance
	{
		unitDistance_km = 0,
		unitDistance_nautical_mile = 1,
	};

}

class ENCCommon
{
public:
	// true = 1  
	// false = 0 (default)
	static bool SymbolizedAreaBoundary;

	// true = 1  
	// false = 0 (default)
	static bool SeabedAreaType;

	static bool TWO_SHADES;
	static bool SHALLOW_PATTERN;

	// if true then show depth of water
	static bool SOUNDING;

	//  if true then show lighthouse light
	static bool LIGHTS;

	//  if ture then show Depth contour
	static bool CONTOUR_LABELS;

	static bool	TEXTOUT;

	static bool	SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER;

	static bool	FULL_SECTORS;

	// if true then apply scale min 
	static bool APPLY_SCALE_MIN;

	// true = show National Object Name 
	static bool SHOW_NOBJNM;


	static bool SHOW_LIST_ABBREVIATION;

	static bool WGS84_TEXT_TYPE;

	static double SAFETY_DEPTH;				// unit : meter [IMP PS [3]]

	static double SAFETY_CONTOUR;			// unit : meter (optional)
	static double SHALLOW_CONTOUR;			// unit : meter [IMP PS [3]]
	static double DEEP_CONTOUR;				// unit : meter (optional)

	static int DISPLAY_FONT_SIZE;

	static unsigned DrawingType;		    // paper  = 1
										// simple = 0
	static __int64 OVER_GROUP;

	static std::wstring DISPLAY_FONT_NAME;

	static int DISPLAY_MODE;
	static int m_eColorTable;
	static int UNIT_DISTANCE;
	static int m_UserMode;

	static std::unordered_map<int, bool> objectDisplaySettings;
	static std::unordered_map<std::wstring, bool> featureDisplaySettings;

	static bool DISPLAY_PLANE;

	static float DISPLAY_SYMBOL_SCALE;

	// true:  center Point(fluid)
	// false: All AREA center point(fixed)
	static bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

	// Show Text Placement
	static BOOL	SHOW_TEXT_PLACEMENT;

	// Under/Over scale change simbol Test
	static int T_APPLY_OVER_N_UNDER_SCALE;
	static int T_CURRENT_SCALE;

	static bool SIMPLIFIED_POINT_SYMBOL;
	static bool AUTOSELECTION_CATALOGUE;
	static bool S111_SHOW_NODATA;
	static bool Show_INFORM01; //inform01 show / hide

public:
	static bool Save(std::wstring filePath);
	static bool Open(std::wstring filePath);
};