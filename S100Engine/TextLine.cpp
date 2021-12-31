#include "stdafx.h"
#include "TextLine.h"

namespace TextPackage 
{
	TextLine::TextLine()
	{

	}

	TextLine::~TextLine()
	{
		if (endOffset)
		{
			delete endOffset;
			endOffset = nullptr;
		}
	}
}
