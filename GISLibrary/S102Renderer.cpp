#include "stdafx.h"
#include "S102Renderer.h"
#include "S102H5.h"
#include "S102_FC_BathymetryCoverage.h"
#include "D2D1Resources.h"
#include "Layer.h"
#include "../GeoMetryLibrary/Scaler.h"

namespace
{
	// Rasterise the depth grid into the dataset's WIC bitmap (once).
	void EnsureBitmap(S102H5& grid, GISLibrary::D2D1Resources* d2)
	{
		if (grid.pWICBitmap || !d2 || !d2->pImagingFactory || !d2->pD2Factory)
		{
			return;
		}

		auto featureContainer = grid.GetBathymetryCoverage();
		if (!featureContainer)
		{
			return;
		}
		auto featureInstance = featureContainer->GetBathymetryCoverage();
		if (!featureInstance)
		{
			return;
		}
		auto valuesGroup = featureInstance->GetBathymetryCoverage();
		if (!valuesGroup)
		{
			return;
		}

		const auto width = featureInstance->getNumPointsLongitudinal();
		const auto height = featureInstance->getNumPointsLatitudinal();

		HRESULT hr = d2->pImagingFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &grid.pWICBitmap);
		if (FAILED(hr) || !grid.pWICBitmap)
		{
			return;
		}

		ID2D1RenderTarget* rt = nullptr;
		hr = d2->pD2Factory->CreateWicBitmapRenderTarget(grid.pWICBitmap, D2D1::RenderTargetProperties(), &rt);
		if (FAILED(hr) || !rt)
		{
			return;
		}

		ID2D1SolidColorBrush* brush = nullptr;
		hr = rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &brush);
		if (SUCCEEDED(hr) && brush)
		{
			rt->BeginDraw();
			const auto numPoint = width * height;
			const D2D1_RECT_F cell = { 0, 0, 1, 1 };
			for (auto i = 0; i < numPoint; i++)
			{
				const auto value = valuesGroup->depth[i];
				if (value == 1000000)
				{
					continue; // no data
				}
				const auto xIndex = i % width;
				const auto yIndex = (height - 1) - i / width;
				rt->SetTransform(D2D1::Matrix3x2F::Translation(static_cast<float>(xIndex), static_cast<float>(yIndex)));
				brush->SetColor(grid.getColor(value));
				brush->SetOpacity(1);
				rt->FillRectangle(cell, brush);
			}
			rt->EndDraw();
			brush->Release();
		}
		rt->Release();
	}
}

namespace GISLibrary
{
	namespace S102Renderer
	{
		void Draw(S102H5& grid, D2D1Resources* d2, HDC& hDC, Scaler* scaler)
		{
			if (!d2 || !scaler || !grid.GetLayer())
			{
				return;
			}

			EnsureBitmap(grid, d2);
			if (!grid.pWICBitmap)
			{
				return;
			}

			auto rt = d2->RenderTarget();
			if (!rt)
			{
				return;
			}

			ID2D1Bitmap* bitmap = nullptr;
			if (FAILED(rt->CreateBitmapFromWicBitmap(grid.pWICBitmap, &bitmap)) || !bitmap)
			{
				return;
			}

			rt->BindDC(hDC, scaler->GetScreenRect());
			rt->BeginDraw();
			rt->SetTransform(D2D1::Matrix3x2F::Identity());

			long sxmin = 0, sxmax = 0, symin = 0, symax = 0;
			auto mbr = grid.GetLayer()->GetMBR();
			scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);
			scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

			rt->DrawBitmap(bitmap,
				D2D1::RectF(static_cast<float>(sxmin), static_cast<float>(symin), static_cast<float>(sxmax), static_cast<float>(symax)),
				1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

			rt->EndDraw();
			bitmap->Release();
		}
	}
}
