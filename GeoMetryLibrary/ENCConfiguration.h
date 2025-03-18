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

	GeoMetryLibrary::DisplayModeTable DISPLAY_MODE;
	GeoMetryLibrary::ColorTable m_eColorTable;

	std::wstring DISPLAY_FONT_NAME;

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

	bool Show_INFORM01 = true;
};