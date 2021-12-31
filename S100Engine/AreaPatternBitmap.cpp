#include "stdafx.h"
#include "AreaPatternBitmap.h"

AreaPatternBitmap::AreaPatternBitmap()
{

}

AreaPatternBitmap::~AreaPatternBitmap()
{
	SafeRelease(&pIWICBitmap);
	SafeRelease(&pBitmapBrush);
}