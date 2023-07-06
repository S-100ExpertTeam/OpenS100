#include "stdafx.h"
#include "AbstractLineStyle.h"
#include "LineStyle.h"
#include "LineStyleReference.h"
#include "CompositeLineStyle.h"
#include "LineSymbol.h"

namespace LineStylesPackage
{
	AbstractLineStyle::AbstractLineStyle()
	{
		subClassType = SubClassTypeOfAbstractLineStyle::eAbstractLineStyle;
	}

	AbstractLineStyle::~AbstractLineStyle()
	{

	}

	int AbstractLineStyle::GetLineStyleCount()
	{
		if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyle)
		{
			LineStyle *pLineStyle = (LineStyle *)this;
			return 1;
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyleReference)
		{
			return 0;
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eCompositeLineStyle)
		{
			CompositeLineStyle *pCompositeLineStyle = (CompositeLineStyle *)this;
			int count = 0;
			for (auto i = pCompositeLineStyle->component.begin(); i != pCompositeLineStyle->component.end(); i++)
			{
				count += (*i)->GetLineStyleCount();
			}

			return count;
		}
		return 0;
	}

	void AbstractLineStyle::GetRect(D2D1_RECT_F *pRect)
	{
		if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyle)
		{
			LineStyle *pLineStyle = (LineStyle *)this;

			pRect->left = 0;
			pRect->right = (FLOAT)(pLineStyle->intervalLength / 0.32);
			
			for (auto i = pLineStyle->symbol.begin(); i != pLineStyle->symbol.end(); i++)
			{
			}
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyleReference)
		{
			// LineStyleReference is not being used.
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eCompositeLineStyle)
		{
			CompositeLineStyle *pCompositeLineStyle = (CompositeLineStyle *)this;
			pRect->left = 0;
			pRect->top = 0;
			pRect->right = 100;
			pRect->bottom = 100;
		}
	}

	LineStyle * AbstractLineStyle::GetLineStyle(int index)
	{
		if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyle)
		{
			LineStyle *pLineStyle = (LineStyle *)this;
			return pLineStyle;
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eLineStyleReference)
		{
			return nullptr;
		}
		else if (subClassType == SubClassTypeOfAbstractLineStyle::eCompositeLineStyle)
		{
			CompositeLineStyle *pCompositeLineStyle = (CompositeLineStyle *)this;

			if ((index < 0) || (index >= (int)pCompositeLineStyle->component.size()))
			{
				return nullptr;
			}
			
			return (LineStyle*)pCompositeLineStyle->component.at(index);
		}
		return nullptr;
	}
}