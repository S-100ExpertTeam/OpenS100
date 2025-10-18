#include "stdafx.h"
#include "SENC_CommonFuc.h"

void ToLower(std::wstring& str)
{
	BOOL bStartExtension = FALSE;

	std::wstring::iterator it = str.begin();
	while (it != str.end())
	{
		// (1) Use unsigned char to process 0 and positive.
		wchar_t c = *it;

		if (bStartExtension)
		{
			bStartExtension = FALSE;
		}
		else
		{
			// (2) Below 128 are alphabets and basic characters.
			if (c < 128 && !bStartExtension)
			{
				*it = tolower(c);
			}
			else
			{
				bStartExtension = TRUE;
			}
		}

		it++;
	}
}

int SENC_CommonFuc::GetRotationCRS(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"geographiccrs"))
	{
		return GEOGRAPHIC_CRS;
	}
	else if (!name.compare(L"portrayalcrs"))
	{
		return PORTRAYAL_CRS;
	}
	else if (!name.compare(L"localcrs"))
	{
		return LOCAL_CRS;
	}
	else if (!name.compare(L"linecrs"))
	{
		return LINE_CRS;
	}
	return 0;
}	

int SENC_CommonFuc::GetRotationCRS(GraphicBasePackage::CRSType value)
{
	if (GraphicBasePackage::CRSType::geographicCRS == value)
	{
		return GEOGRAPHIC_CRS;
	}
	else if (GraphicBasePackage::CRSType::portrayalCRS == value)
	{
		return PORTRAYAL_CRS;
	}
	else if (GraphicBasePackage::CRSType::localCRS == value)
	{
		return LOCAL_CRS;
	}
	else if (GraphicBasePackage::CRSType::lineCRS == value)
	{
		return LINE_CRS;
	}
	
	return 0;
}

int SENC_CommonFuc::GetCapStyle(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"butt"))
	{
		return BUTT;
	}
	else if (!name.compare(L"square"))
	{
		return SQUARE;
	}
	else if (!name.compare(L"round"))
	{
		return ROUND;
	}
	return 0;
}

int SENC_CommonFuc::GetJoinStyle(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"bebel"))
	{
		return BEBEL;
	}
	else if (!name.compare(L"miter"))
	{
		return MITER;
	}
	else if (!name.compare(L"round"))
	{
		return ROUND;
	}
	return 0;
}
int SENC_CommonFuc::GetAreaCRSType(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"global"))
	{
		return GLOBAL;
	}
	else if (!name.compare(L"localgeometry"))
	{
		return LOCAL_GEOMETRY;
	}
	else if (!name.compare(L"globalgeometry"))
	{
		return GLOBAL_GEOMETRY;
	}

	return 0;
}

int SENC_CommonFuc::GetHorizontalAlignment(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"start"))
	{
		return START;
	}
	else if (!name.compare(L"left"))
	{
		return START;
	}
	else if (!name.compare(L"end"))
	{
		return END;
	}
	else if (!name.compare(L"right"))
	{
		return END;
	}
	else if (!name.compare(L"center"))
	{
		return CENTER;
	}
	return START;
}

int SENC_CommonFuc::GetVerticalAlignment(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"top"))
	{
		return TOP;
	}
	else if (!name.compare(L"bottom"))
	{
		return BOTTOM;
	}
	else if (!name.compare(L"center"))
	{
		return CENTER;
	}
	return 0;
}

int SENC_CommonFuc::GetFontWeight(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"right"))
	{
		return LIGHT;
	}
	else if (!name.compare(L"medium"))
	{
		return MEDIUM;
	}
	else if (!name.compare(L"bold"))
	{
		return BOLD;
	}
	return 0;
}

int SENC_CommonFuc::GetFontSlant(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"uplight"))
	{
		return UPRIGHT;
	}
	else if (!name.compare(L"italics"))
	{
		return ITALICS;
	}
	return 0;
}

int SENC_CommonFuc::GetFontProportion(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"monospaces"))
	{
		return MONOSPACES;
	}
	else if (!name.compare(L"proportional"))
	{
		return PROPORTIONAL;
	}
	return 0;
}

int SENC_CommonFuc::GetLinePlacement(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"relative"))
	{
		return RELATIVE;
	}
	else if (!name.compare(L"absolute"))
	{
		return ABSOLUTE;
	}
	return 0;
}

int SENC_CommonFuc::GetAreaPlacement(std::wstring name)
{
	ToLower(name);
	if (!name.compare(L"visibleparts"))
	{
		return VISIBLE_PART;
	}
	else if (!name.compare(L"geographic"))
	{
		return GEOGRAPHIC;
	}
	return 0;
}