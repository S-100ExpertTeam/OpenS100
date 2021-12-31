#include "stdafx.h"
#include "S100DrawingUnitAugmentedPath.h"

S100DrawingUnitAugmentedPath::S100DrawingUnitAugmentedPath()
{

}

S100DrawingUnitAugmentedPath::~S100DrawingUnitAugmentedPath()
{
	SafeRelease(&_pGeometry);
}
