#include "stdafx.h"
#include "AreaFill.h"
#include "AugmentedArea.h"

AugmentedArea::AugmentedArea()
{
	pAreaFill = nullptr;
}

AugmentedArea::~AugmentedArea()
{
	delete pAreaFill;
}
