#include "stdafx.h"
#include "D2D1Resources.h"
#include "GISLibrary.h"

#include "..\\GeoMetryLibrary\\ENCCommon.h"

namespace GISLibrary
{

	D2D1Resources::D2D1Resources()
	{

	}

	D2D1Resources::~D2D1Resources()
	{

	}

	bool D2D1Resources::CreateDeviceIndependentResources()
	{
		CoInitialize(NULL);

		HRESULT hr = S_OK;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2Factory);
		if (!pD2Factory)
		{
			// Failed to create Direct2D Factory! (%02X) (was a debug trace)
			return false;
		}
		
		if (pD2Factory)
		{
			hr = pD2Factory->CreateStrokeStyle(
				D2D1::StrokeStyleProperties1(
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_LINE_JOIN_ROUND,
					10.0f,
					D2D1_DASH_STYLE_SOLID,
					0.0f,
					D2D1_STROKE_TRANSFORM_TYPE_FIXED),
				NULL,
				0,
				&pSolidStrokeStyle);

			if (!pSolidStrokeStyle)
			{
				// Failed to create stroke line!(User Layer Solid Style) (%02X) (was a debug trace)
				return false;
			}

			D2D1StrokeStyleGroup.push_back(pSolidStrokeStyle);

			hr = pD2Factory->CreateStrokeStyle(
				D2D1::StrokeStyleProperties1(
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_LINE_JOIN_ROUND,
					10.0f,
					D2D1_DASH_STYLE_DASH,
					0.0f,
					D2D1_STROKE_TRANSFORM_TYPE_FIXED),
				NULL,
				0,
				&pDashStrokeStyle);

			if (!pDashStrokeStyle)
			{
				// Failed to create stroke line!(User Layer Dash Style) (%02X) (was a debug trace)
				return false;
			}

			D2D1StrokeStyleGroup.push_back(pDashStrokeStyle);

			hr = pD2Factory->CreateStrokeStyle(
				D2D1::StrokeStyleProperties1(
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_LINE_JOIN_ROUND,
					10.0f,
					D2D1_DASH_STYLE_DOT,
					0.0f,
					D2D1_STROKE_TRANSFORM_TYPE_FIXED),
				NULL,
				0,
				&pDotStrokeStyle);

			if (!pDotStrokeStyle)
			{
				// Failed to create stroke line!(User Layer Dot Style) (%02X) (was a debug trace)
				return false;
			}
			D2D1StrokeStyleGroup.push_back(pDotStrokeStyle);


			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), reinterpret_cast<IUnknown **>(&pDWriteFactory));

			if (!pDWriteFactory)
			{
				// Failed to create IDWriteFactory! (%02X) (was a debug trace)
				return false;
			}

			// Create a DirectWrite text format object.
			hr = pDWriteFactory->CreateTextFormat(
				ENCCommon::getDisplayFontNameW().c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				(float)ENCCommon::DISPLAY_FONT_SIZE,
				L"", //locale
				&pDWriteTextFormat
			);

			pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

			if (!pDWriteTextFormat)
			{
				// Failed to create IDWriteTextFormat! (%02X) (was a debug trace)
				return false;
			}

			// Create a DirectWrite text format object.
			hr = pDWriteFactory->CreateTextFormat(
				L"hy gothic-extra",
				NULL,
				DWRITE_FONT_WEIGHT_BOLD,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				(float)ENCCommon::DISPLAY_FONT_SIZE,
				L"", //locale
				&pDWriteTextFormatArea
			);

			pDWriteTextFormatArea->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			pDWriteTextFormatArea->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			pDWriteTextFormatArea->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

			if (!pDWriteTextFormatArea)
			{
				// Failed to create IDWriteTextFormat (for Area)! (%02X) (was a debug trace)
				return false;
			}

			if (SUCCEEDED(hr))
			{
				hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					(LPVOID*)&pImagingFactory);
			}

			return true;
		}

		return false;
	}


	void D2D1Resources::DeleteDeviceIndependentResources()
	{
		SafeRelease(&pD2Factory);
		SafeRelease(&pSolidStrokeStyle);
		SafeRelease(&pDashStrokeStyle);
		SafeRelease(&pDotStrokeStyle);
		SafeRelease(&pDWriteFactory);
		SafeRelease(&pDWriteTextFormat);
		SafeRelease(&pDWriteTextFormatArea);
		SafeRelease(&pDWriteTextLayout);
	}


	bool D2D1Resources::CreateDeviceDependentResources()
	{
		HRESULT hr = S_OK;

		if (!pRT)
		{
			// Create a DC render target.
			D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_IGNORE),
				0,
				0,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT);

			// Create a Direct2D render target.
			hr = pD2Factory->CreateDCRenderTarget(
				&props,
				&pRT);

			if (SUCCEEDED(hr))
			{
				hr = pRT->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Crimson),
					&pBrush
				);

				if (SUCCEEDED(hr))
				{
					return true;
				}
				else
				{
					// Failed to create ID2D1SolidColorBrush! (%02X) (was a debug trace)
					return false;
				}
			}
			else
			{
				// Failed to create ID2D1DCRenderTarget! (%02X) (was a debug trace)
				return false;
			}
		}

		return false;
	}


	void D2D1Resources::DeleteDeviceDependentResources()
	{
		SafeRelease(&pRT);
		SafeRelease(&pBrush);
	}


	bool D2D1Resources::CreateReSizeTextFormat(int /*size*/)
	{
		SafeRelease(&pDWriteTextFormat);

		pDWriteFactory->CreateTextFormat(
				L"rounded typeface",
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				20,
				L"", //locale
				&pDWriteTextFormat
			);

		if (!pDWriteTextFormat)
		{
			// Failed to create IDWriteTextFormat! (%02X) (was a debug trace)
			return false;
		}

		pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

		return true;
	}

	ID2D1Factory1* D2D1Resources::Factory()
	{
		return pD2Factory;
	}

	IDWriteFactory* D2D1Resources::WriteFactory()
	{
		return pDWriteFactory;
	}

	IDWriteTextFormat* D2D1Resources::WriteTextFormat()
	{
		return pDWriteTextFormat;
	}

	ID2D1StrokeStyle1* D2D1Resources::SolidStrokeStyle()
	{
		return pSolidStrokeStyle;
	}

	ID2D1DCRenderTarget* D2D1Resources::RenderTarget()
	{
		return pRT;
	}

	ID2D1SolidColorBrush* D2D1Resources::SolidColorBrush()
	{
		return pBrush;
	}

	void D2D1Resources::Begin(HDC& hdc, const RECT& rect)
	{
		pRT->BindDC(hdc, &rect);
		pRT->BeginDraw();
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void D2D1Resources::End()
	{
		pRT->EndDraw();
	}

	void D2D1Resources::DrawRect(const RECT& rect)
	{
		D2D1_RECT_F d2rect = {
			(FLOAT)rect.left, (FLOAT)rect.top, (FLOAT)rect.right, (FLOAT)rect.bottom
		};

		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Crimson));

		D2D1::Matrix3x2F oldMatrix;
		pRT->GetTransform(&oldMatrix);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->DrawRectangle(d2rect, pBrush);
		pRT->SetTransform(oldMatrix);
	}

	void D2D1Resources::DrawD2Rect(D2D1_RECT_F& rect)
	{
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Crimson));

		D2D1::Matrix3x2F oldMatrix;
		pRT->GetTransform(&oldMatrix);
		pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		pRT->DrawRectangle(rect, pBrush);
		pRT->SetTransform(oldMatrix);
	}

	IDWriteTextFormat* D2D1Resources::getWriteTextFormat(int fontSize)
	{
		auto item = writeTextFormatListByFontSize.find(fontSize);

		if (item == writeTextFormatListByFontSize.end())
		{
			IDWriteTextFormat* newWriteTextFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				ENCCommon::getDisplayFontNameW().c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				(float)fontSize,
				L"", //locale
				&newWriteTextFormat
			);

			writeTextFormatListByFontSize.insert(std::make_pair(fontSize, newWriteTextFormat));
			return newWriteTextFormat;
		}

		return item->second;
	}

	IDWriteTextFormat* D2D1Resources::getSlantWriteTextFormat(int fontSize)
	{
		auto item = slantTextFormatListByFontSize.find(fontSize);

		if (item == slantTextFormatListByFontSize.end())
		{
			IDWriteTextFormat* newWriteTextFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				ENCCommon::getDisplayFontNameW().c_str(),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_ITALIC,
				DWRITE_FONT_STRETCH_NORMAL,
				(float)fontSize,
				L"", //locale
				&newWriteTextFormat
			);

			slantTextFormatListByFontSize.insert(std::make_pair(fontSize, newWriteTextFormat));
			return newWriteTextFormat;
		}

		return item->second;
	}
}