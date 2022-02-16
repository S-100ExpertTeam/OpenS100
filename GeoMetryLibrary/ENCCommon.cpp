#include "StdAfx.h"
#include "ENCCommon.h"
#include "ENCGeometry.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"
#include "..\\LibMFCUtil\\StringTokenizer.h"

#include <fstream>

unsigned ENCCommon::DrawingType = 0;		    // 1 :¡®main_PaperChart.xsl¡¯- includes all the COMMON entries plus PAPER_CHART symbols and SYMBOLIZED_BOUNDARIES.
									// 2 :¡®main_SimpleSymbols¡¯ - includes all the COMMON entries plus SIMPLIFIED symbols and SYMBOLIZED _BOUNDARIES.	
									// 3 :¡®main_Simplified.xsl¡¯- includes all the COMMON entries plus SIMPLIFIED symbols and PLAIN_BOUNDARIES.
bool ENCCommon::SymbolizedAreaBoundary = true;
bool ENCCommon::SeabedAreaType = false;

bool   ENCCommon::SOUNDING = false;
bool   ENCCommon::LIGHTS = true;
bool   ENCCommon::CONTOUR_LABELS = true;
bool   ENCCommon::TEXTOUT = true;
bool   ENCCommon::APPLY_SCALE_MIN = true;
bool   ENCCommon::SHOW_NOBJNM = false;
bool   ENCCommon::SHOW_LIST_ABBREVIATION = false;
bool   ENCCommon::WGS84_TEXT_TYPE = true;
/*
** Initial values
** Reference.
   - S-52 Appendix 2
	 12.2.20 Conditional Symbology Procedure 'SEABEDnn'
*/
double ENCCommon::SAFETY_DEPTH = 30;
double ENCCommon::SAFETY_CONTOUR = 30;	// unit : meter
double ENCCommon::SHALLOW_CONTOUR = 10;
double ENCCommon::DEEP_CONTOUR = 30;
bool ENCCommon::DISPLAY_PLANE = false;
bool   ENCCommon::TWO_SHADES = false;
bool   ENCCommon::SHALLOW_PATTERN = false;

bool   ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER = true;

bool ENCCommon::FULL_SECTORS = false;

int   ENCCommon::INTEROPERABILITY_LEVEL = 0;
int   ENCCommon::T_APPLY_OVER_N_UNDER_SCALE = 1;
int   ENCCommon::T_CURRENT_SCALE = 0;


bool ENCCommon::DETECTION_MODE = false;

std::unordered_map<int, bool> ENCCommon::objectDisplaySettings;
std::unordered_map<std::wstring, bool> ENCCommon::featureDisplaySettings;

int ENCCommon::DISPLAY_MODE = GeoMetryLibrary::DisplayModeTable::all;
int ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Day; //thema
int ENCCommon::UNIT_DISTANCE = GeoMetryLibrary::UnitDistance::unitDistance_km;
int ENCCommon::m_UserMode = GeoMetryLibrary::UserMode::User_Mode;


__int64 ENCCommon::OVER_GROUP = 0x7FFFFFFF;

BOOL ENCCommon::SHOW_TEXT_PLACEMENT = FALSE;

std::wstring ENCCommon::DISPLAY_FONT_NAME = L"Malgun Gothic";
int ENCCommon::DISPLAY_FONT_SIZE = 15;

float ENCCommon::DISPLAY_SYMBOL_SCALE = 5;
bool   ENCCommon::APPLY_DATA_COVERAGE_SCALE = false;

bool   ENCCommon::AREA_SYMBOL_DYNAMIC_POSITION_MODE = true;

#define OVERGROUP_CON1	(viewGroup >= 10000 && viewGroup <= 19999) || (viewGroup >= 40000 && viewGroup <= 49999)
#define OVERGROUP_CON2	viewGroup == 21010
#define OVERGROUP_CON3	viewGroup >= 21020 && viewGroup <= 21060
#define OVERGROUP_CON4	viewGroup >= 22010 && viewGroup <= 22260
#define OVERGROUP_CON5	viewGroup == 23010
#define OVERGROUP_CON6	viewGroup >= 23020 && viewGroup <= 24010 
#define OVERGROUP_CON7	viewGroup >= 25010 && viewGroup <= 25040  
#define OVERGROUP_CON8	viewGroup >= 26010 && viewGroup <= 26050 
#define OVERGROUP_CON9  viewGroup >= 26210 && viewGroup <= 26270 
#define OVERGROUP_CON10 viewGroup >= 27010 && viewGroup <= 27050 
#define OVERGROUP_CON11	viewGroup == 27070
#define OVERGROUP_CON12 viewGroup == 27080
#define OVERGROUP_CON13 viewGroup >= 27210 && viewGroup <= 27230 
#define OVERGROUP_CON14 viewGroup >= 28010 && viewGroup <= 28020 
#define OVERGROUP_CON15 viewGroup >= 52010 && viewGroup <= 52030
#define OVERGROUP_CON16 viewGroup >= 52210 && viewGroup <= 52240
#define OVERGROUP_CON17	viewGroup == 52410 && viewGroup <= 52440
#define OVERGROUP_CON18 viewGroup == 52460
#define OVERGROUP_CON19 viewGroup >= 53010 && viewGroup <= 53080
#define OVERGROUP_CON20 viewGroup >= 54010 && viewGroup <= 54030
#define OVERGROUP_CON21 viewGroup >= 55010 && viewGroup <= 55020
#define OVERGROUP_CON22 viewGroup >= 56000 && viewGroup <= 56999
#define OVERGROUP_CON23 viewGroup >= 31010 && viewGroup <= 31070
#define OVERGROUP_CON24 viewGroup >= 32010 && viewGroup <= 32460
#define OVERGROUP_CON25 viewGroup == 33010 
#define OVERGROUP_CON26 (viewGroup >= 33020 && viewGroup <= 33060) || viewGroup == 31080
#define OVERGROUP_CON27 viewGroup >= 34010 && viewGroup <= 34070
#define OVERGROUP_CON28 viewGroup >= 38010 && viewGroup <= 38210
#define OVERGROUP_CON29 viewGroup >= 61010 && viewGroup <= 61050
#define OVERGROUP_CON30 viewGroup >= 62010 && viewGroup <= 62020
#define OVERGROUP_CON31 true


bool ENCCommon::SIMPLIFIED_POINT_SYMBOL = false;
bool ENCCommon::AUTOSELECTION_CATALOGUE = true;
bool ENCCommon::S111_SHOW_NODATA = false;
bool ENCCommon::Show_INFORM01 = true;
bool ENCCommon::S111_THINNING = false;

bool ENCCommon::Save(std::wstring filePath)
{
	std::locale::global(std::locale("Korean"));
	std::ofstream ofs;
	ofs.open(L"..\\ProgramData\\data\\settings.txt", std::ios::out | std::ios::trunc | std::ios::binary);

	std::string strTrue = "true";
	std::string strFalse = "false";

	std::string t = "*This is comment which is not effective on settings.\n";
	ofs.write(t.c_str(), t.size());

	t = "APPLY_SCALE_MIN\t";
	ENCCommon::APPLY_SCALE_MIN ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "CONTOUR_LABELS\t";
	ENCCommon::CONTOUR_LABELS ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "DEEP_CONTOUR\t";
	t.append(_bstr_t(ENCCommon::DEEP_CONTOUR));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "DISPLAY_MODE\t";
	t.append(_bstr_t((int)ENCCommon::DISPLAY_MODE));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "DrawingType\t";
	t.append(_bstr_t(ENCCommon::DrawingType));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "LIGHTS\t";
	ENCCommon::LIGHTS ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "OVER_GROUP\t";
	t.append(_bstr_t(ENCCommon::OVER_GROUP));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SAFETY_DEPTH\t";
	t.append(_bstr_t(ENCCommon::SAFETY_DEPTH));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SAFETY_CONTOUR\t";
	t.append(_bstr_t(ENCCommon::SAFETY_CONTOUR));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SHALLOW_CONTOUR\t";
	t.append(_bstr_t(ENCCommon::SHALLOW_CONTOUR));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "DISPLAY_FONT_NAME\t";

	char* path = LibMFCUtil::ConvertWCtoC((wchar_t*)std::wstring(ENCCommon::DISPLAY_FONT_NAME).c_str());
	t.append(path);
	delete path;
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "DISPLAY_FONT_SIZE\t";
	t.append(_bstr_t(ENCCommon::DISPLAY_FONT_SIZE));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SHALLOW_PATTERN\t";
	ENCCommon::SHALLOW_PATTERN ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER\t";
	ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "FULL_SECTORS\t";
	ENCCommon::FULL_SECTORS ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SHOW_NOBJNM\t";
	ENCCommon::SHOW_NOBJNM ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SHOW_LIST_ABBREVIATION\t";
	ENCCommon::SHOW_LIST_ABBREVIATION ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "WGS84_TEXT_TYPE\t";
	ENCCommon::WGS84_TEXT_TYPE ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SOUNDING\t";
	ENCCommon::SOUNDING ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SymbolizedAreaBoundary\t";
	ENCCommon::SymbolizedAreaBoundary ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "SeabedType\t";
	ENCCommon::SeabedAreaType ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "TEXTOUT\t";
	ENCCommon::TEXTOUT ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "TWO_SHADES\t";
	ENCCommon::TWO_SHADES ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "AUTOSELECTION_CATALOGUE\t";
	ENCCommon::AUTOSELECTION_CATALOGUE ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	//write Setting 
	t = "m_eColorTable\t";
	t.append(_bstr_t((int)ENCCommon::m_eColorTable));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "UNIT_DISTANCE\t";
	t.append(_bstr_t((int)ENCCommon::UNIT_DISTANCE));
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	t = "S111_SHOW_NODATA\t";
	ENCCommon::S111_SHOW_NODATA ? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	//Show infor01
	t = "Show_INFORM01\t";
	ENCCommon::Show_INFORM01? t.append(strTrue) : t.append(strFalse);
	t.append("\n");
	ofs.write(t.c_str(), t.size());

	std::unordered_map<int, bool>::iterator oitor;

	t = "OBJECT_SHOW_SETTING_BEGIN\n";
	ofs.write(t.c_str(), t.size());

	for (oitor = ENCCommon::objectDisplaySettings.begin(); oitor != ENCCommon::objectDisplaySettings.end(); oitor++)
	{
		t = "\t";
		t.append(_bstr_t((*oitor).first));
		t.append("\t");
		(*oitor).second ? t.append("1\n") : t.append("0\n");
		ofs.write(t.c_str(), t.size());
	}

	t = "OBJECT_SHOW_SETTING_END\n";
	ofs.write(t.c_str(), t.size());

	t = "OBJECT_SHOW_SETTING_S101_BEGIN\n";
	ofs.write(t.c_str(), t.size());

	t = "OBJECT_SHOW_SETTING_S101_END\n";
	ofs.write(t.c_str(), t.size());

	ofs.close();

	return true;
}

bool ENCCommon::Open(std::wstring filePath)
{
	//project load => Repaint
	ENCCommon::objectDisplaySettings.clear();  // init setting
	
	std::ifstream ifs;
	ifs.open(filePath, std::ios::in);

	if (!ifs.is_open())
	{
		OutputDebugString(_T("Can not load (settings.txt)"));
		return false;
	}

	std::string strTrue = "true";
	std::string strFalse = "false";
	std::string strLine;

	while (getline(ifs, strLine))
	{
		std::string opt = strLine.substr(0, 1);
		if (opt.compare("*") == 0)
			continue;

		StringTokenizer *pstringTokenizer = new StringTokenizer(strLine, " \t");
		strLine.clear();

		if (pstringTokenizer->hasMoreTokens())
		{
			std::string token = pstringTokenizer->nextToken();

			if (token.compare("APPLY_SCALE_MIN") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::APPLY_SCALE_MIN = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::APPLY_SCALE_MIN = false;
					}
				}
			}
			else if (token.compare("CONTOUR_LABELS") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::CONTOUR_LABELS = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::CONTOUR_LABELS = false;
					}
				}
			}
			else if (token.compare("DEEP_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::DEEP_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("DISPLAY_MODE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::DISPLAY_MODE = static_cast<GeoMetryLibrary::DisplayModeTable>(atoi(token.c_str()));
				}
			}
			else if (token.compare("DrawingType") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::DrawingType = atoi(token.c_str());
				}
			}
			else if (token.compare("LIGHTS") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::LIGHTS = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::LIGHTS = false;
					}
				}
			}
			else if (token.compare("OVER_GROUP") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::OVER_GROUP = _atoi64(token.c_str());
				}
			}
			else if (token.compare("SAFETY_DEPTH") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::SAFETY_DEPTH = atof(token.c_str());
				}
			}
			else if (token.compare("SAFETY_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::SAFETY_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("DISPLAY_FONT_NAME") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					auto fontName = LibMFCUtil::ConvertCtoWC((char*)token.c_str());
					ENCCommon::DISPLAY_FONT_NAME = fontName;
					delete[] fontName;
				}
			}
			else if (token.compare("DISPLAY_FONT_SIZE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::DISPLAY_FONT_SIZE = atoi(token.c_str());
				}
			}
			else if (token.compare("SHALLOW_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::SHALLOW_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("SHALLOW_PATTERN") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SHALLOW_PATTERN = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SHALLOW_PATTERN = false;
					}
				}
			}
			else if (token.compare("SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER = false;
					}
				}
			}
			else if (token.compare("FULL_SECTORS") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::FULL_SECTORS = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::FULL_SECTORS = false;
					}
				}
			}
			else if (token.compare("SHOW_NOBJNM") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SHOW_NOBJNM = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SHOW_NOBJNM = false;
					}
				}
			}
	
			else if (token.compare("SHOW_LIST_ABBREVIATION") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SHOW_LIST_ABBREVIATION = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SHOW_LIST_ABBREVIATION = false;
					}
				}
			}
			else if (token.compare("WGS84_TEXT_TYPE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::WGS84_TEXT_TYPE = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::WGS84_TEXT_TYPE = false;
					}
				}
			}
			else if (token.compare("SOUNDING") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SOUNDING = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SOUNDING = false;
					}
				}
			}
			else if (token.compare("SymbolizedAreaBoundary") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SymbolizedAreaBoundary = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SymbolizedAreaBoundary = false;
					}
				}
			}
			else if (token.compare("SeabedType") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::SeabedAreaType = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::SeabedAreaType = false;
					}
				}
			}
			else if (token.compare("TEXTOUT") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::TEXTOUT = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::TEXTOUT = false;
					}
				}
			}
			else if (token.compare("TWO_SHADES") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::TWO_SHADES = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::TWO_SHADES = false;
					}
				}
			}
			else if (token.compare("AUTOSELECTION_CATALOGUE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::AUTOSELECTION_CATALOGUE = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						ENCCommon::AUTOSELECTION_CATALOGUE = false;
					}
				}
			}
			//Load Day Dusk Night.
			else if (token.compare("m_eColorTable") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::m_eColorTable = static_cast<GeoMetryLibrary::ColorTable>(atoi(token.c_str()));

					if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Day)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Day;
					}
					else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Dusk)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Dusk;
					}
					else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Night)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Night;
					}
				}
			}
			else if (token.compare("UNIT_DISTANCE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::UNIT_DISTANCE = static_cast<GeoMetryLibrary::UnitDistance>(atoi(token.c_str()));
				}
			}
			else if (token.compare("S111_SHOW_NODATA") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					if (pstringTokenizer->hasMoreTokens())
					{
						token = pstringTokenizer->nextToken();
						if (token.compare(strTrue) == 0)
						{
							ENCCommon::S111_SHOW_NODATA = true;
						}
						else if (token.compare(strFalse) == 0)
						{
							ENCCommon::S111_SHOW_NODATA = false;
						}
					}
				}
			}
			else if (token.compare("Show_INFORM01")==0) 
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						ENCCommon::Show_INFORM01 = true;
					}
					else  if (token.compare(strFalse) == 0)
					{
						ENCCommon::Show_INFORM01 = false;
					}
				}
			}
			else if (token.compare("OBJECT_SHOW_SETTING_BEGIN") == 0)
			{
				getline(ifs, strLine);
				while (strLine.compare("OBJECT_SHOW_SETTING_END") != 0)
				{
					int objectCode = 0;
					bool objectValue = 0;

					StringTokenizer *pstringTokenizerObjSettings = new StringTokenizer(strLine, " \t");

					if (pstringTokenizerObjSettings->hasMoreTokens())
					{
						token = pstringTokenizerObjSettings->nextToken();

						objectCode = atoi(token.c_str());
						if (pstringTokenizerObjSettings->hasMoreTokens())
						{
							token = pstringTokenizerObjSettings->nextToken();

							int value = atoi(token.c_str());

							value ? objectValue = true : objectValue = false;
						}
					}

					delete pstringTokenizerObjSettings;

					if (objectCode > 0)
					{
						std::unordered_map<int, bool>::iterator ositor;

						ositor = ENCCommon::objectDisplaySettings.find(objectCode);
						if (ositor != ENCCommon::objectDisplaySettings.end())
						{
							ositor->second = objectValue;

						}
					}
					getline(ifs, strLine);
				}
			}
		}
		delete pstringTokenizer;
	}
	return true;
}