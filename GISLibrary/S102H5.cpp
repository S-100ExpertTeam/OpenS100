#include "stdafx.h"
#include "S102H5.h"
#include "S102_RootGroup.h"
#include "S102_FC_BathymetryCoverage.h"
#include "GISLibrary.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include <hdf5.h>

S102H5::S102H5()
{
	type = S100SpatialObjectType::S102H5;
	featureContainer.push_back(new S102_FC_BathymetryCoverage());
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

void S102H5::Draw(HDC& hDC, Scaler* scaler, double offset)
{
	if (!pWICBitmap) {
		CreateBitmap();
	}

	if (pWICBitmap) {
		auto rt = gisLib->D2.RenderTarget();

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

		//rt->FillRectangle(rect, brush);

		auto mbr = GetLayer()->GetMBR();
		auto map = scaler->GetMap();

		scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);
		scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

		rt->DrawBitmap(pBitmap, D2D1::RectF(sxmin, symin, sxmax, symax), 1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

		if (!std::isnan(map.xmin)) {
			MBR clippedMBR;
			if (MBR::IntersectMBR(mbr, map, clippedMBR)) {
				clippedMBR.xmin = 0;
			}
		}

		//rt->FillRectangle(rect, gisLib->D2.pBrush);

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
	if (!gisLib->s102Color.isSet()) {
		gisLib->s102Color.setColor();
	}

	auto featureContainer = GetBathymetryCoverage();
	auto featureInstance = featureContainer->GetBathymetryCoverage();
	auto valuesGroup = featureInstance->GetBathymetryCoverage();

	auto width = featureInstance->getNumPointsLongitudinal();
	auto height = featureInstance->getNumPointsLatitudinal();

	HRESULT hr = gisLib->D2.pImagingFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPRGBA, WICBitmapCacheOnDemand, &pWICBitmap);
	if (SUCCEEDED(hr)) {
		ID2D1RenderTarget* rt = nullptr;
		hr = gisLib->D2.pD2Factory->CreateWicBitmapRenderTarget(
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
			
			auto numPoint = width * height;
			D2D1_RECT_F rect = { 0, 0, 1, 1 };
			for (auto i = 0; i < numPoint; i++) {

				auto xIndex = i % width;
				auto yIndex = (height - 1) - i / width;

				auto value = valuesGroup->depth[i];

				if (value != 1000000) {

					rt->SetTransform(D2D1::Matrix3x2F::Identity());
					rt->SetTransform(D2D1::Matrix3x2F::Translation(xIndex, yIndex));

					brush->SetColor(getColor(value));

					rt->FillRectangle(rect, brush);
				}
			}

			rt->EndDraw();

			SafeRelease(&brush);
		}

		SafeRelease(&rt);
	}

	//auto regularGrid = s102->GetRegularGrid(0);
	//if (regularGrid)
	//{
	//	auto featureValue = regularGrid->GetRegularGridFeatureValues(0);
	//	auto gridInfo = regularGrid->GetRegularGridInformation();
	//	if (gridInfo && featureValue)
	//	{
	//		auto floatValue = featureValue->GetFloatValues(0);
	//		if (!floatValue)
	//		{
	//			return;
	//		}

	//		float* values = floatValue->GetValues();

	//		int ncols = gridInfo->GetNumPointLongitudinal();
	//		int nrows = gridInfo->GetNumPointLatitudinal();

	//		if (bmpGrid.GetSafeHandle())
	//		{
	//			dc.DeleteDC();
	//			bmpGrid.DeleteObject();
	//			bBMP = false;
	//		}

	//		if (!bBMP)
	//		{
	//			if (bmpGrid.CreateBitmap(ncols, nrows, 1, 32, NULL) == 0)
	//			{
	//				return;
	//			}

	//			dc.CreateCompatibleDC(pDC);
	//			dc.SelectObject(bmpGrid);

	//			BITMAP pBitmap;
	//			bmpGrid.GetBitmap(&pBitmap);

	//			int* bmpBuffer = new int[pBitmap.bmWidth * pBitmap.bmHeight];
	//			memset(bmpBuffer, 0, sizeof(int) * (pBitmap.bmWidth * pBitmap.bmHeight));

	//			float gridOriginX = gridInfo->GetGridOgirinLongitude();
	//			float gridOriginY = gridInfo->GetGridOriginLatitude();
	//			float dx = gridInfo->GetGridSpacingLongitudinal();
	//			float dy = gridInfo->GetGridSpacingLatitudinal();

	//			for (int irow = 0; irow < nrows; irow++)
	//			{
	//				for (int icol = 0; icol < ncols; icol++)
	//				{
	//					int index = (nrows - 1 - irow) * ncols + icol;

	//					if (values[(irow * ncols) + icol] == 1000000.0 ||
	//						values[(irow * ncols) + icol] == -1000000.0)
	//					{
	//						bmpBuffer[index] = RGB(255, 255, 255);

	//						continue;
	//					}

	//					double elevation = values[(irow * ncols) + icol];

	//					if (elevation > 100000)
	//					{
	//						int a = 0;
	//					}

	//					if (APPLY_TIDE)
	//					{
	//						elevation += (TIDAL / 100.0);
	//					}

	//					bmpBuffer[index] = GetColour(elevation);
	//				}
	//			}

	//			bmpGrid.SetBitmapBits(pBitmap.bmWidthBytes * pBitmap.bmHeight, bmpBuffer);

	//			delete[] bmpBuffer;

	//			bBMP = true;
	//		}
	//	}
	//}

	//Close();
}

void S102H5::DeleteBitmap()
{
	SafeRelease(&pWICBitmap);
}

D2D1_COLOR_F S102H5::getColor(float depth)
{
	if (depth > ENCCommon::DEEP_CONTOUR) {
		return gisLib->s102Color.DEPDW[0];
	}
	else if (depth > ENCCommon::SAFETY_CONTOUR) {
		return gisLib->s102Color.DEPMD[0];
	}
	else if (depth > ENCCommon::SHALLOW_CONTOUR) {
		return gisLib->s102Color.DEPMS[0];
	}
	else if (depth > 0) {
		return gisLib->s102Color.DEPVS[0];
	}
	else {
		return gisLib->s102Color.DEPIT[0];
	}

	return D2D1::ColorF(D2D1::ColorF::Black);
}

void S102H5::SetPositive()
{
	if (rootGroup->productSpecification.find("2.0.0") != std::string::npos) {
		auto featureContainer = GetBathymetryCoverage();
		auto featureInstance = featureContainer->GetBathymetryCoverage();
		auto valuesGroup = featureInstance->GetBathymetryCoverage();

		auto width = featureInstance->getNumPointsLongitudinal();
		auto height = featureInstance->getNumPointsLatitudinal();

		auto numPoint = width * height;

		for (int i = 0; i < numPoint; i++) {
			if (valuesGroup->depth[i] != 1000000) {
				valuesGroup->depth[i] = -valuesGroup->depth[i];
			}
		}
	}
}