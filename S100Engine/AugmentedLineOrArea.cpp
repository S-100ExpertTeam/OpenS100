#include "stdafx.h"
#include "AugmentedLineOrArea.h"
#include "LineStyle.h"

AugmentedLineOrArea::AugmentedLineOrArea()
{

}

AugmentedLineOrArea::~AugmentedLineOrArea()
{
	if (pLineStyle)
	{
		delete pLineStyle;
		pLineStyle = nullptr;
	}
}