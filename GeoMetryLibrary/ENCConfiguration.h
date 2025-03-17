#pragma once
#include "ENCCommon.h"

class ENCConfiguration
{
public:
	ENCConfiguration();
	virtual ~ENCConfiguration();

public:
	bool TEXTOUT;

	bool FULL_SECTORS;

	// if true then apply scale min 
	bool APPLY_SCALE_MIN;

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

	GeoMetryLibrary::DisplayModeTable DISPLAY_MODE;
	GeoMetryLibrary::ColorTable m_eColorTable;

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

	float DISPLAY_SYMBOL_SCALE;

	// true:  center Point(fluid)
	// false: All AREA center point(fixed)
	bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

	bool SIMPLIFIED_POINT_SYMBOL;

	bool AUTOSELECTION_CATALOGUE = false;

	bool S111_SHOW_NODATA = false;

	bool Show_INFORM01 = true;
};