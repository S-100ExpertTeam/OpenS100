#include "stdafx.h"
#include "S100Utilities.h"
#include "GISLibrary.h"
#include "SCommonFuction.h"

#include <string>
#include <filesystem>
#include <cctype>

ScaleBand S100Utilities::scaleBand[15] = {
		ScaleBand(INT_MAX, 10000000),
		ScaleBand(10000000, 3500000),
		ScaleBand(3500000, 1500000),
		ScaleBand(1500000, 700000),
		ScaleBand(700000, 350000),
		ScaleBand(350000, 180000),
		ScaleBand(180000, 90000),
		ScaleBand(90000, 45000),
		ScaleBand(45000, 22000),
		ScaleBand(22000, 12000),
		ScaleBand(12000, 8000), 
		ScaleBand(8000, 4000),
		ScaleBand(4000, 3000),
		ScaleBand(3000, 2000),
		ScaleBand(2000, 1000)
};




D2D1::ColorF S100Utilities::GetColorNum(int num)
{
	std::vector<D2D1::ColorF> colors;

	colors.push_back(D2D1::ColorF::Red); 
	colors.push_back(D2D1::ColorF::Lime);
	colors.push_back(D2D1::ColorF::Blue);
	colors.push_back(D2D1::ColorF::Yellow);
	colors.push_back(D2D1::ColorF::Cyan);
	colors.push_back(D2D1::ColorF::Magenta);
	colors.push_back(D2D1::ColorF::OrangeRed);
	colors.push_back(D2D1::ColorF::DeepPink);
	colors.push_back(D2D1::ColorF::DodgerBlue);
	colors.push_back(D2D1::ColorF::Chartreuse);
	colors.push_back(D2D1::ColorF::SpringGreen);
	colors.push_back(D2D1::ColorF::DarkOrange);
	colors.push_back(D2D1::ColorF::DeepSkyBlue);
	colors.push_back(D2D1::ColorF::HotPink);
	colors.push_back(D2D1::ColorF::RoyalBlue);

	if (num >= 0 && num < colors.size())
		return colors[num];
	else
		return D2D1::ColorF::Black;
}




ScaleBand S100Utilities::GetLegacyScaleband(int scale) 
{
	switch (scale)
	{
	case 1:
		return ScaleBand(300000, 5000000);
	case 2:
		return ScaleBand(150000, 300000);
	case 3:
		return ScaleBand(50000, 150000);
	case 4:
		return ScaleBand(20000, 50000);
	case 5:
		return ScaleBand(5000, 20000);
	case 6:
		return ScaleBand(2000, 5000);
	default:
		return ScaleBand(0, 0);
	}
};


int S100Utilities::GetLevel(std::wstring path)
{
	// get without extension
	std::wstring name = std::filesystem::path(path).stem();
	if (name.length() >= 8)
	{
		auto levelStr = name.substr(7, 1);
		if (std::isdigit(levelStr.at(0)))
		{
			auto level = std::stoi(levelStr);
			return level;
		}
	}

	return 0;
}

// S-98_Main_Document_2.3.0 Ver
int S100Utilities::GetScaleBand(int scale)
{
	if (scale > scaleBand[0].GetOptimumDisplayScale()) {
		return 0;
	}

	for (int i = 1; i <= 14; i++) {
		if (scale > scaleBand[i].GetOptimumDisplayScale() &&
			scale <= scaleBand[i].GetMinimumDisplayScale()) {
			return i;
		}
	}

	return 14;
}

// S-98_Main_Document_2.3.0 Ver
std::vector<int> S100Utilities::algorithm_ScaleBand(S100::DataCoverage dataCoverage)
{
	int minDS = *dataCoverage.MinimumDisplayScale;
	int optDS = *dataCoverage.OptimumDisplayScale;
	std::vector<int> S;

	if (minDS > scaleBand[0].GetOptimumDisplayScale()) {
		S.push_back(0);
	}

	for (int i = 1; i <= 14; i++) {
		if (min(minDS, scaleBand[i].GetMinimumDisplayScale()) > max(optDS, scaleBand[i].GetOptimumDisplayScale())) {
			S.push_back(i);
		}
	}

	return S;
}

// S-98_Main_Document_2.3.0 Ver
std::vector<int> S100Utilities::GetScaleBand(ScaleBand sb)
{
	int minDS = sb.GetMinimumDisplayScale();
	int maxDS = sb.GetOptimumDisplayScale();
	std::vector<int> S;

	if (maxDS < scaleBand[0].GetOptimumDisplayScale()) {
		S.push_back(0);
	}

	for (int i = 1; i <= 14; i++) {
		if (max(maxDS, scaleBand[i].GetOptimumDisplayScale()) < min(minDS, scaleBand[i].GetMinimumDisplayScale())) {
			S.push_back(i);
		}
	}
	return S;
}

std::vector<std::shared_ptr<InventoryItem>> S100Utilities::SelectDataCoverages(std::vector<std::shared_ptr<Inventory>> INV, Scaler* scaler, MBR viewport)
{
	bool first = true;

	// 1. S = 0
	std::vector<std::shared_ptr<InventoryItem>> S;
	
	if (std::isnan(viewport.GetWidth()))
		return S;

	long lonTemp;
	long latTemp;
	ClipperLib::Path  view;
	ClipperLib::Paths  viewPaths;

	ClipperLib::IntPoint tmp;

	// View port
	double lon = viewport.GetXMin();
	double lat = viewport.GetYMin();
	scaler->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;  
	tmp.Y = latTemp;
	view.push_back(tmp);
	
	lon = viewport.GetXMax();
	lat = viewport.GetYMin();
	scaler->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;   
	tmp.Y = latTemp;
	view.push_back(tmp);
	
	lon = viewport.GetXMax();
	lat = viewport.GetYMax();
	scaler->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;   
	tmp.Y = latTemp;
	view.push_back(tmp);

	lon = viewport.GetXMin();
	lat = viewport.GetYMax();
	scaler->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;  
	tmp.Y = latTemp;
	view.push_back(tmp);

	viewPaths.push_back(view);

	// 2. ScaleBand = GetScaleBand(scale)
	int SB = GetScaleBand(scaler->GetCurrentScale());

	// 3. While viewPort != 0 do
	while (viewPaths.size() != 0)
	{
		// a. For all dataCoverage in collection
		for (auto item : INV)
		{
			// i. If scaleBand ...
			bool scaleBandInScaleBand = false;
			//algorithm_ScaleBand();

			for (int i = 0; i < item->vecScaleRange.size(); i++)
			{
				auto scaleBandIndices = GetScaleBand(item->vecScaleRange[i]);
				if (std::find(scaleBandIndices.begin(), scaleBandIndices.end(), SB) != scaleBandIndices.end())
				{
					if (SCommonFuction::IntersectionPaths(viewPaths, item->vecBoundingPolygon[i], scaler))
					{
						// Set DataCoverage Log
						if (first)
						{
							OutputDebugStringA(" \n");
							OutputDebugStringA("===== Select DataCoverages =====\n");
							first = false;
						}

						std::string str;
						str += "FileName :";
						str += item->strFileName;
						str += " \n";
						str += "Index: ";
						str += std::to_string(i);
						str += " \n";
						str += "OptDisplayScale : ";
						str += std::to_string(item->vecScaleRange[i].GetOptimumDisplayScale());
						str += " \n";
						str += "MinDisplayScale : ";
						str += std::to_string(item->vecScaleRange[i].GetMinimumDisplayScale());
						str += " \n";
						str += " \n";
						OutputDebugStringA(str.c_str());

						///////////////////////

						std::shared_ptr<InventoryItem> inventoryitem = std::make_shared<InventoryItem>();
						inventoryitem->strFileName = item->strFileName;
						inventoryitem->mbrBoundingBox = item->mbrBoundingBox;
						inventoryitem->BoundingPolygon = item->vecBoundingPolygon[i];
						inventoryitem->ScaleRange = item->vecScaleRange[i];
						inventoryitem->strFilePath = item->strFilePath;

						S.push_back(inventoryitem);
						viewPaths = SCommonFuction::ClipPaths(viewPaths, item->vecBoundingPolygon[i], scaler);
					}
				}
			}
		}

		// b. ScaleBand = ScaleBand - 1
		SB = SB - 1;

		// c. If ScaleBand = 0
		if (SB < 0) {
			// i. Return S
			return S;
		}
	}

	// 4. Return S
	return S; 
}


