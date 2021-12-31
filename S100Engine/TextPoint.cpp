#include "stdafx.h"
#include "TextPoint.h"

namespace TextPackage 
{
	TextPoint::TextPoint()
	{

	}

	TextPoint::~TextPoint()
	{
		if (offset)
		{
			delete offset;
			offset = nullptr;
		}
	}
}
