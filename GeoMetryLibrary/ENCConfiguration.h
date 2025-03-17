#pragma once
#include "ENCCommon.h"

class ENCConfiguration
{
public:
	ENCConfiguration();
	virtual ~ENCConfiguration();

public:
	// true = 1  
	// false = 0 (default)
	bool SymbolizedAreaBoundary;

	bool TWO_SHADES;
	bool SHALLOW_PATTERN;

	// if true then show depth of water
	bool SOUNDING;

	//  if true then show lighthouse light
	bool LIGHTS;

	//  if ture then show Depth contour
	bool CONTOUR_LABELS;

	bool TEXTOUT;

	bool SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER;

	bool FULL_SECTORS;

	// if true then apply scale min 
	bool APPLY_SCALE_MIN;

	// true = show National Object Name 
	bool SHOW_NOBJNM;
	bool SHOW_LIST_ABBREVIATION;
	bool WGS84_TEXT_TYPE;
	

	// unit : meter
	double SAFETY_DEPTH;

	// unit : meter
	double SAFETY_CONTOUR;
	double SHALLOW_CONTOUR;
	double DEEP_CONTOUR;

	int DISPLAY_FONT_SIZE;

	// 1 :‘main_PaperChart.xsl’- includes all the COMMON entries plus PAPER_CHART symbols and SYMBOLIZED_BOUNDARIES.
	// 2 :‘main_SimpleSymbols’ - includes all the COMMON entries plus SIMPLIFIED symbols and SYMBOLIZED _BOUNDARIES.
	// 3 :‘main_Simplified.xsl’- includes all the COMMON entries plus SIMPLIFIED symbols and PLAIN_BOUNDARIES.
	int DrawingType;

	__int64 OVER_GROUP;

	GeoMetryLibrary::UserMode m_UserMode;
	GeoMetryLibrary::DisplayModeTable DISPLAY_MODE;
	GeoMetryLibrary::ColorTable m_eColorTable;
	GeoMetryLibrary::UnitDistance UNIT_DISTANCE;

	std::wstring DISPLAY_FONT_NAME;

	std::unordered_map<int, bool> objectDisplaySettings;

	std::unordered_map<std::wstring, bool> featureDisplaySettings;

private:
	bool fontChanged = false;
	bool colorTableChanged = false;
	void FontChanged();
	void ColorTableChanged();

public:
	bool Open(std::string path);
	bool Save(std::string path);

	// enccommon
	bool DISPLAY_PLANE;

	float DISPLAY_SYMBOL_SCALE;

	// true:  center Point(fluid)
	// false: All AREA center point(fixed)
	bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

   // Show Text Placement
	BOOL	SHOW_TEXT_PLACEMENT;

	// Under/Over scale change simbol Test
	int T_APPLY_OVER_N_UNDER_SCALE;
	int T_CURRENT_SCALE;

	bool SIMPLIFIED_POINT_SYMBOL;

	bool AUTOSELECTION_CATALOGUE = false;

	bool S111_SHOW_NODATA = false;

	bool Show_INFORM01 = true;
};