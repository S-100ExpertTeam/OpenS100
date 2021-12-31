#pragma once

class AreaPatternBitmap
{
public:
	AreaPatternBitmap();
	virtual ~AreaPatternBitmap();

public:
	IWICBitmap* pIWICBitmap = nullptr;
	ID2D1BitmapBrush* pBitmapBrush = nullptr;
};