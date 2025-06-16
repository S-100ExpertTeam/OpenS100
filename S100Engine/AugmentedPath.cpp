#include "stdafx.h"
#include "AugmentedPath.h"
#include "Path.h"

AugmentedPath::AugmentedPath()
{

}

AugmentedPath::~AugmentedPath()
{
	if (pPath)
	{
		delete pPath;
		pPath = nullptr;
	}
}