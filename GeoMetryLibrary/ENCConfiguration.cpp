#include "stdafx.h"
#include "ENCConfiguration.h"

#include "..\\LibMFCUtil\\StringTokenizer.h"

#include <fstream>
#include <string>

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


ENCConfiguration::ENCConfiguration()
{

}

ENCConfiguration::~ENCConfiguration()
{

}

bool ENCConfiguration::Open(std::string path)
{
	std::ifstream ifs;
	ifs.open(path, std::ios::in);

	if (!ifs.is_open())
	{
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
					FontChanged();
					delete[] fontName;
				}
			}
			else if (token.compare("DISPLAY_FONT_SIZE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::DISPLAY_FONT_SIZE = atoi(token.c_str());
					FontChanged();
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
			//Load Day Dusk Night
			else if (token.compare("m_eColorTable") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					ENCCommon::m_eColorTable = static_cast<GeoMetryLibrary::ColorTable>(atoi(token.c_str()));

					if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Day)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Day;
						ColorTableChanged();
					}
					else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Dusk)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Dusk;
						ColorTableChanged();
					}
					else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Night)
					{
						ENCCommon::m_eColorTable = GeoMetryLibrary::ColorTable::Night;
						ColorTableChanged();
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
					}
					getline(ifs, strLine);
				}
			}
		}
		delete pstringTokenizer;
	}

	return true;
}

bool ENCConfiguration::Save(std::string path)
{
	return true;
}

void ENCConfiguration::FontChanged()
{
	fontChanged = true;
}

void ENCConfiguration::ColorTableChanged()
{
	colorTableChanged = true;
}