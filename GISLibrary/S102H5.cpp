#include "stdafx.h"
#include "S102H5.h"
#include "S102_RootGroup.h"
#include "S102_FC_BathymetryCoverage.h"
#include "GISLibrary.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include <hdf5.h>

S102H5::S102H5(PortrayalCatalogue* pc, D2D1Resources* d2d1) : S100H5(d2d1)
{
	type = S100SpatialObjectType::S102H5;
	featureContainer.push_back(new S102_FC_BathymetryCoverage());
	m_ObejctType = SpatialObjectType::S102H5;

	if (!s102Color.isSet()) {
		s102Color.setColor(pc);
	}
}

S102H5::~S102H5()
{
	DeleteBitmap();
}

bool S102H5::Open(CString _filepath)
{
	SetFilePath(_filepath);
	SetFileName(LibMFCUtil::GetFileName(_filepath));

	auto path = pugi::as_utf8(_filepath);

	auto fileID = H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	if (fileID < 0)
	{
		return false;
	}

	rootGroup = new S102_RootGroup();

	rootGroup->Read(fileID);

	auto bathymetryCoverage = GetBathymetryCoverage();
	if (bathymetryCoverage)
	{
		auto groupID = H5Gopen(fileID, "BathymetryCoverage", H5P_DEFAULT);
		if (groupID > 0)
		{
			if (!bathymetryCoverage->Read(groupID))
			{
				H5Fclose(fileID);
				return false;
			}
		}
		else
		{
			H5Fclose(fileID);
			return false;
		}
	}

	SetPositive();

	H5Fclose(fileID);

	SetMBR();

	return true;
}

bool S102H5::OpenMetadata(CString _filepath)
{
	return true;
}

void S102H5::Draw(HDC& hDC, Scaler* scaler, double offset)
{
	if (!pWICBitmap) {
		CreateBitmap();
	}

	if (pWICBitmap) {
		auto rt = D2->RenderTarget();

		ID2D1Bitmap* pBitmap = nullptr;
		auto hr = rt->CreateBitmapFromWicBitmap(pWICBitmap, &pBitmap);
		ID2D1BitmapBrush* brush = nullptr;
		hr = rt->CreateBitmapBrush(pBitmap, &brush);

		if (SUCCEEDED(hr))
		{
			brush->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
			brush->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);
			brush->SetInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		}

		hr = rt->BindDC(hDC, scaler->GetScreenRect());
		rt->BeginDraw();

		rt->SetTransform(D2D1::Matrix3x2F::Identity());

		D2D1_RECT_F rect = { 10.0f, 10.0f, 1000.0f, 1000.0f };

		long sxmin = 0;
		long sxmax = 0;
		long symin = 0;
		long symax = 0;

		auto mbr = GetLayer()->GetMBR();
		auto map = scaler->GetMap();

		scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);
		scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

		rt->DrawBitmap(pBitmap, D2D1::RectF((float)sxmin, (float)symin, (float)sxmax, (float)symax), 1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

		hr = rt->EndDraw();

		SafeRelease(&brush);
		SafeRelease(&pBitmap);
	}
}

S102_FC_BathymetryCoverage* S102H5::GetBathymetryCoverage()
{
	return (S102_FC_BathymetryCoverage*)featureContainer.at(0);
}

void S102H5::CreateBitmap()
{
	auto featureContainer = GetBathymetryCoverage();
	auto featureInstance = featureContainer->GetBathymetryCoverage();
	auto valuesGroup = featureInstance->GetBathymetryCoverage();

	auto width = featureInstance->getNumPointsLongitudinal();
	auto height = featureInstance->getNumPointsLatitudinal();

	// The grid is walked below through valuesGroup->depth. Bound that walk by
	// what Read() actually allocated instead of recomputing width * height,
	// which is int arithmetic over file-supplied attributes and wraps for large
	// declared dimensions.
	if (!valuesGroup || !valuesGroup->depth || valuesGroup->pointCountRead == 0) {
		return;
	}

	if (width <= 0 || height <= 0 ||
		static_cast<size_t>(width) * static_cast<size_t>(height) != valuesGroup->pointCountRead) {
		return;
	}

	HRESULT hr = D2->pImagingFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &pWICBitmap);
	if (SUCCEEDED(hr)) {
		ID2D1RenderTarget* rt = nullptr;
		hr = D2->pD2Factory->CreateWicBitmapRenderTarget(
			pWICBitmap, 
			D2D1::RenderTargetProperties(), 
			&rt);

		if (SUCCEEDED(hr)) {
			ID2D1SolidColorBrush* brush = nullptr;
			auto hr = rt->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Crimson),
				&brush
			);

			rt->BeginDraw();
			
			const size_t numPoint = valuesGroup->pointCountRead;
			const size_t gridWidth = static_cast<size_t>(width);
			const size_t gridHeight = static_cast<size_t>(height);
			D2D1_RECT_F rect = { 0, 0, 1, 1 };
			for (size_t i = 0; i < numPoint; i++) {

				auto xIndex = i % gridWidth;
				auto yIndex = (gridHeight - 1) - i / gridWidth;

				auto value = valuesGroup->depth[i];

				if (value != 1000000) {

					rt->SetTransform(D2D1::Matrix3x2F::Identity());
					rt->SetTransform(D2D1::Matrix3x2F::Translation((float)xIndex, (float)yIndex));

					brush->SetColor(getColor(value));
					brush->SetOpacity(1);

					rt->FillRectangle(rect, brush);
				}
			}

			rt->EndDraw();

			SafeRelease(&brush);
		}

		SafeRelease(&rt);
	}
}

void S102H5::DeleteBitmap()
{
	SafeRelease(&pWICBitmap);
}

D2D1_COLOR_F S102H5::getColor(float depth)
{
	if (depth > ENCCommon::DEEP_CONTOUR) {
		return s102Color.DEPDW[0];
	}
	else if (depth > ENCCommon::SAFETY_CONTOUR) {
		return s102Color.DEPMD[0];
	}
	else if (depth > ENCCommon::SHALLOW_CONTOUR) {
		return s102Color.DEPMS[0];
	}
	else if (depth > 0) {
		return s102Color.DEPVS[0];
	}
	else {
		return s102Color.DEPIT[0];
	}

	return D2D1::ColorF(D2D1::ColorF::Black);
}

void S102H5::SetPositive()
{
	if (rootGroup->productSpecification.find("2.0.0") != std::string::npos) {
		auto featureContainer = GetBathymetryCoverage();
		auto featureInstance = featureContainer->GetBathymetryCoverage();
		auto valuesGroup = featureInstance->GetBathymetryCoverage();

		// Same reasoning as CreateBitmap(): iterate over what was read, not over
		// a product recomputed from the file's dimension attributes.
		if (!valuesGroup || !valuesGroup->depth) {
			return;
		}

		const size_t numPoint = valuesGroup->pointCountRead;

		for (size_t i = 0; i < numPoint; i++) {
			if (valuesGroup->depth[i] != 1000000) {
				valuesGroup->depth[i] = -valuesGroup->depth[i];
			}
		}
	}
}