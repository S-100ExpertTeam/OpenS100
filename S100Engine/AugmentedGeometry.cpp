#include "stdafx.h"
#include "AugmentedGeometry.h"
#include "CRSType.h"
#include "Text.h"

AugmentedGeometry::AugmentedGeometry()
{

}

AugmentedGeometry::~AugmentedGeometry()
{
	if (pCRSType)
	{
		delete pCRSType;
		pCRSType = nullptr;
	}

	if (pText)
	{
		delete pText;
		pText = nullptr;
	}

	if (_text)
	{
		delete _text;
		_text = nullptr;
	}
}