#pragma once

#include "S100H5.h"
#include "S102_FC_BathymetryCoverage.h"

class S102H5 : 
	public S100H5
{
public:
	S102H5();
	virtual ~S102H5();

public:
	IWICBitmap* pWICBitmap = nullptr;
	//ID2D1Bitmap* pBitmap = nullptr;

public:
	bool Open(CString _filepath) override;
	void Draw(HDC& hDC, Scaler* scaler, double offset = 0) override;

	S102_FC_BathymetryCoverage* GetBathymetryCoverage();

	void CreateBitmap();
	void DeleteBitmap();
	D2D1_COLOR_F getColor(float depth);
	void SetPositive();
};

