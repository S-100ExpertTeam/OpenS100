#include "stdafx.h"
#include "CSS.h"
#include "UnitOfCSS.h"
#include "CSSFill.h"
#include "CSSStroke.h"
#include "CSSStrokeLinecap.h"
#include "CSSStrokeLinejoin.h"
#include "CSSStrokeWidth.h"
#include "CSSDisplay.h"
#include "CSSOpacity.h"
#include "Declaration.h"
#include "CSSStyle.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"
#include "..\\LibMFCUtil\\StringTokenizer.h"

#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <list>
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	CSS::CSS()
	{

	}


	CSS::~CSS()
	{

	}

	bool CSS::ReadCSSFile(std::wstring path)
	{
		std::ifstream ifs;
		ifs.open(path.c_str(), std::ios::in);

		if (!ifs.is_open())
		{
			return false;
		}

		std::string strTrue = "true";
		std::string strFalse = "false";
		std::string strLine;

		std::string command = "";

		// Read all data.
		while (getline(ifs, strLine))
		{
			command += strLine;
		}

		// delete commant
		size_t seekBegin = command.find("/*");
		size_t seekEnd = command.find("*/");
		while (seekBegin != -1)
		{
			command.erase(seekBegin, seekEnd - seekBegin + 2);
			seekBegin = command.find("/*");
			seekEnd = command.find("*/");
		}

		seekBegin = command.find("{");
		seekEnd = command.find("}");

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		while (seekBegin != -1)
		{
			std::string tokSelector = command.substr(0, seekBegin);
			std::string tokDeclarations = command.substr(seekBegin + 1, seekEnd - seekBegin - 1);
			std::string tokDeclaration;
			std::string tokProperty;
			std::string tokValue;
			command.erase(0, seekEnd + 1);
			if (tokSelector.length() > 0)
			{
				size_t beg = tokSelector.find_first_not_of("\a\b\f\n\r\t\v ");
				size_t end = tokSelector.find_last_not_of("\a\b\f\n\r\t\v ");

				tokSelector = std::string(tokSelector, beg, end - beg + 1);
			}
			else
			{
				continue;
			}
			UnitOfCSS* unit = new UnitOfCSS();

			auto dot = tokSelector.find('.');
			if (dot != std::string::npos)
			{
				tokSelector = tokSelector.substr(1, tokSelector.size() - 1);
			}

			unit->selector = converter.from_bytes(tokSelector);


			std::unordered_map<int, int> m_tt;
			m_tt.insert(std::unordered_map<int, int>::value_type(1, 2));

			m_unitMap.insert(std::pair<std::wstring, UnitOfCSS*>(unit->selector, unit));
			StringTokenizer *pstringTokenizer1 = new StringTokenizer(tokDeclarations, ";");
			while (pstringTokenizer1->hasMoreTokens())
			{
				tokDeclaration = pstringTokenizer1->nextToken();
				StringTokenizer *pstringTokenizer2 = new StringTokenizer(tokDeclaration, ":");

				if (pstringTokenizer2->hasMoreTokens())
				{
					tokProperty = pstringTokenizer2->nextToken();

					if (pstringTokenizer2->hasMoreTokens())
					{
						tokValue = pstringTokenizer2->nextToken();
					}
				}

				Declaration dec;
				dec.propertyName = converter.from_bytes(tokProperty); //file
				dec.value = converter.from_bytes(tokValue); //color

				unit->declaration.push_back(dec); 

				delete pstringTokenizer2;

			}

			delete pstringTokenizer1;

			seekBegin = command.find("{");
			seekEnd = command.find("}");
		}
		OrganizeCSS();// CSSize and store values that are stored in m_unitMap.
		return true;
	}

	bool CSS::OrganizeCSS()
	{
		std::unordered_map<std::wstring, UnitOfCSS*>::iterator  hitor;
		std::list<Declaration>::iterator itor;

		CSSStyle* cssStyle = NULL;

		for (hitor = m_unitMap.begin(); hitor != m_unitMap.end(); hitor++)
		{
			UnitOfCSS* unitCSS = hitor->second;

			for (itor = unitCSS->declaration.begin(); itor != unitCSS->declaration.end(); itor++)
			{
				Declaration* dec = &(*itor);

				cssStyle = GetCSSStyle(dec->propertyName, dec->value);

				unitCSS->styles.push_back(cssStyle);
			}
		}
		return true;
	}


	CSSStyle* CSS::GetCSSStyle(std::wstring _propertyName, std::wstring _value)
	{
		CSSStyle* cssStyle = NULL;
		if (_propertyName.compare(L"fill") == 0)
		{
			CSSFill *css = new CSSFill();
			cssStyle = (CSSStyle*)css;
			if (_value.compare(L"none") == 0) {
				css->bFill = false;
			}
			else {
				std::wstring str = _value.substr(1, _value.size() - 1);
				int tInteger = (int)_wcstoi64(str.c_str(), 0, 16);
				css->color = tInteger;
			}
		}
		else if (_propertyName.compare(L"stroke") == 0)
		{
			CSSStroke *css = new CSSStroke();
			cssStyle = (CSSStyle*)css;
			std::wstring str = _value.substr(0, 1);
			if (str.compare(L"#") != 0)
			{
				if (_value.compare(L"black") == 0)
				{
					css->color = 0x000000;
				}
				else if (_value.compare(L"blue") == 0)
				{
					css->color = 0x0000FF;
				}
				else if (_value.compare(L"red") == 0)
				{
					css->color = 0xFF0000;
				}
			}
			else
			{
				str = _value.substr(1, _value.size() - 1);
				int tInteger = (int)_wcstoi64(str.c_str(), 0, 16);
				css->color = tInteger;
			}
		}
		else if (_propertyName.compare(L"stroke-width") == 0)
		{
			CSSStrokeWidth *css = new CSSStrokeWidth();
			cssStyle = (CSSStyle*)css;

			css->width = (float)_wtof(_value.c_str());
		}
		else if (_propertyName.compare(L"stroke-linecap") == 0)
		{
			CSSStrokeLinecap *css = new CSSStrokeLinecap();
			cssStyle = (CSSStyle*)css;
			if (_value.compare(L"round") == 0)
			{
				css->type = 2;
			}
		}
		else if (_propertyName.compare(L"stroke-linejoin") == 0)
		{
			CSSStrokeLinejoin *css = new CSSStrokeLinejoin();
			cssStyle = (CSSStyle*)css;
			if (_value.compare(L"round") == 0)
			{
				css->type = 2;
			}
		}
		else if (_propertyName.compare(L"display") == 0)
		{
			CSSDisplay *css = new CSSDisplay();
			cssStyle = (CSSStyle*)css;
			if (_value.compare(L"none") == 0)
			{
				css->bDisplay = false;
			}
		}
		else if (_propertyName.compare(L"fill-opacity") == 0)
		{
			CSSOpacity *css = new CSSOpacity();
			cssStyle = (CSSOpacity*)css;
			css->opacity = (float)_wtof(_value.c_str());
		}
		else
		{
			auto css = new CSSStyle();
			cssStyle = css;
		}
		return cssStyle;
	}
}
