#pragma once

#include <vector>

class ENCCell;

struct ID2D1Factory1;
struct IDWriteFactory;
struct IDWriteTextFormat;
struct ID2D1StrokeStyle1;
struct ID2D1DCRenderTarget;
struct ID2D1SolidColorBrush;

namespace GISLibrary
{
	class D2D1Resources
	{
	private:
		ID2D1StrokeStyle1 *pSolidStrokeStyle = nullptr;
		ID2D1StrokeStyle1 *pDashStrokeStyle = nullptr;
		ID2D1StrokeStyle1 *pDotStrokeStyle = nullptr;
	public:
		D2D1Resources();
		virtual ~D2D1Resources();

		// Device Independet Resources
		ID2D1Factory1 *pD2Factory = nullptr;
		
		/*
		Stroke Style Group
		0: Solid
		1: Dash
		2: Dot
		*/
		std::vector<ID2D1StrokeStyle1*> D2D1StrokeStyleGroup;

		IDWriteFactory *pDWriteFactory = nullptr;
		IDWriteTextFormat *pDWriteTextFormat = nullptr;
		IDWriteTextFormat* pDWriteTextFormatArea = nullptr;
		IDWriteTextLayout *pDWriteTextLayout = nullptr;
		IWICImagingFactory* pImagingFactory = nullptr;

		std::unordered_map<int, IDWriteTextFormat*> writeTextFormatListByFontSize;
		std::unordered_map<int, IDWriteTextFormat*> slantTextFormatListByFontSize;

		// Device Dependent Resources
		ID2D1DCRenderTarget *pRT = nullptr; // direct 2D
		ID2D1SolidColorBrush *pBrush = nullptr;

	public:
		bool CreateDeviceIndependentResources();
		void DeleteDeviceIndependentResources();

		bool CreateDeviceDependentResources();
		void DeleteDeviceDependentResources();

		bool CreateReSizeTextFormat(int size);

		ID2D1Factory1* Factory();
		IDWriteFactory* WriteFactory();
		IDWriteTextFormat* WriteTextFormat();
		ID2D1StrokeStyle1* SolidStrokeStyle();
		ID2D1DCRenderTarget* RenderTarget();
		ID2D1SolidColorBrush* SolidColorBrush();

		void Begin(HDC& hdc, CRect& rect);
		void End();

		void DrawCRect(CRect& rect);
		void DrawD2Rect(D2D1_RECT_F& rect);

		IDWriteTextFormat* getWriteTextFormat(int fontSize);
		IDWriteTextFormat* getSlantWriteTextFormat(int fontSize);
	};
}