#pragma once

#include "S100H5.h"
#include "S102_Color.h"
#include "S102_FC_BathymetryCoverage.h"

class S102H5 : 
	public S100H5
{
public:
	S102H5(PortrayalCatalogue* pc);
	virtual ~S102H5();

public:
	// Render cache filled by GISLibrary::S102Renderer (to move out of the dataset in a later step).
	IWICBitmap* pWICBitmap = nullptr;
	//ID2D1Bitmap* pBitmap = nullptr;
	S102_Color s102Color;

public:
	bool Open(const std::wstring& _filepath) override;
	bool OpenMetadata(const std::wstring& _filepath) override;

	S102_FC_BathymetryCoverage* GetBathymetryCoverage();

	void DeleteBitmap();
	D2D1_COLOR_F getColor(float depth);
	void SetPositive();
};

