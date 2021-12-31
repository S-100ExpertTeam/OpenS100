#include "stdafx.h"
#include "LineStyles.h"
#include "LineStyle.h"
#include "LineSymbol.h"
#include "CompositeLineStyle.h"

namespace LineStylesPackage
{
	LineStyles::LineStyles()
	{
	}
	LineStyles::~LineStyles()
	{
		for (auto i = mapLineStyle.begin(); i != mapLineStyle.end(); i++)
		{
			delete i->second;
		}
	}
}


bool LineStylesPackage::LineStyles::GetRect(CString name, D2D1_RECT_F *pRect)
{
	auto keyValue = mapLineStyle.find(std::wstring(name));
	if (keyValue != mapLineStyle.end())
	{
		keyValue->second->GetRect(pRect);
		return true;
	}

	return false;
}