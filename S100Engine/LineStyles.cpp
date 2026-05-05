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


bool LineStylesPackage::LineStyles::GetRect(const std::string& name, D2D1_RECT_F* pRect)
{
	auto keyValue = mapLineStyle.find(name);
	if (keyValue != mapLineStyle.end())
	{
		keyValue->second->GetRect(pRect);
		return true;
	}
	return false;
}

bool LineStylesPackage::LineStyles::GetRect(const std::wstring& name, D2D1_RECT_F* pRect)
{
	return GetRect(LibMFCUtil::WStringToString(name), pRect);
}