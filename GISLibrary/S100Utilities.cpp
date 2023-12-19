#include "stdafx.h"
#include "S100Utilities.h"
#include "GISLibrary.h"
#include "SCommonFuction.h"

#include <string>
#include <filesystem>
#include <cctype>

ScaleBands S100Utilities::scaleBands[15] = {
		ScaleBands(-1, 10000000),
		ScaleBands(10000000, 3500000),
		ScaleBands(3500000, 1500000),
		ScaleBands(1500000, 700000),
		ScaleBands(700000, 350000),
		ScaleBands(350000, 180000),
		ScaleBands(180000, 90000),
		ScaleBands(90000, 45000),
		ScaleBands(45000, 22000),
		ScaleBands(22000, 12000),
		ScaleBands(12000, 8000),
		ScaleBands(8000, 4000),
		ScaleBands(4000, 3000),
		ScaleBands(3000, 2000),
		ScaleBands(2000, 1000)
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

int S100Utilities::GetScaleBand(int scale)
{
	if (scale > scaleBands[0].maximumScale) {
		return 0;
	}

	for (int i = 1; i <= 14; i++) {
		if (scaleBands[i].minimumScale >= scale && scale > scaleBands[i].maximumScale) {
			return i;
		}
	}

	return 14;
}

std::vector<int> S100Utilities::GetScaleBands(S100::DataCoverage dataCoverage)
{
	int minDS = *dataCoverage.MinimumDisplayScale;
	int maxDS = *dataCoverage.MaximumDisplayScale;
	std::vector<int> S;

	if (minDS < scaleBands[0].maximumScale) {
		S.push_back(0);
	}

	for (int i = 1; i <= 14; i++) {
		if (max(minDS, scaleBands[i].minimumScale) < min(maxDS, scaleBands[i].maximumScale)) {
			S.push_back(i);
		}
	}

	return S;
}


std::vector<std::shared_ptr<InventoryItem>> S100Utilities::SelectDataCoverages(std::vector<std::shared_ptr<Inventory>> INV, int scale, MBR viewport)
{
	bool first = true;

	

	std::vector<std::shared_ptr<InventoryItem>> S;
	
	if (std::isnan(viewport.GetWidth()))
		return S;

	long lonTemp;
	long latTemp;
	ClipperLib::Path  view;
	ClipperLib::Paths  viewPaths;

	ClipperLib::Path  view1;
	ClipperLib::Paths  viewPaths1;

	ClipperLib::IntPoint tmp;

	// View port
	double lon = viewport.GetXMin();
	double lat = viewport.GetYMin();
	gisLib->GetScaler()->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;  
	tmp.Y = latTemp;
	view.push_back(tmp);
	
	lon = viewport.GetXMax();
	lat = viewport.GetYMin();
	gisLib->GetScaler()->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;   
	tmp.Y = latTemp;
	view.push_back(tmp);
	
	lon = viewport.GetXMax();
	lat = viewport.GetYMax();
	gisLib->GetScaler()->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;   
	tmp.Y = latTemp;
	view.push_back(tmp);

	lon = viewport.GetXMin();
	lat = viewport.GetYMax();
	gisLib->GetScaler()->WorldToDevice(lon, lat, &lonTemp, &latTemp);
	tmp.X = lonTemp;  
	tmp.Y = latTemp;
	view.push_back(tmp);

	viewPaths.push_back(view);

	
	int SB = GetScaleBand(scale);

	while (viewPaths.size() != 0)
	{
		for (auto item : INV)
	{
			for (int i = 0; i < item->vecScaleRange.size(); i++)
			{
				if (item->vecScaleRange[i].isIntersection(scaleBands[SB].minimumScale, scaleBands[SB].maximumScale))
				{
					if (SCommonFuction::IntersectionPaths(viewPaths, item->vecBoundingPolygon[i]))
					{
						// Set DataCoverage Log
						if (first)
						{
							OutputDebugStringA(" \n");
							OutputDebugStringA("===== Select DataCoverages =====\n");
							first = false;
						}
						
						std::string str = "";
						str += "FileName :";
						str += item->strFileName;
						str += " \n";
						str += "Index: ";
						str += std::to_string(i);
						str += " \n";
						str += "MaxDisplayScale : ";
						str += std::to_string(item->vecScaleRange[i].MaxDisplayScale);
						str += " \n";
						str += "MinDisplayScale : ";
						str += std::to_string(item->vecScaleRange[i].MinDisplayScale);
						str += " \n";
						str += " \n";
						OutputDebugStringA(str.c_str());

						///////////////////////

						std::shared_ptr<InventoryItem> inventoryitem = std::make_shared<InventoryItem>();
							inventoryitem->strFileName = item->strFileName;
							inventoryitem->mbrBoundingBox = item->mbrBoundingBox;
							inventoryitem->BoundingPolygon = item->vecBoundingPolygon[i];
							inventoryitem->ScaleRange = item->vecScaleRange[i];

							S.push_back(inventoryitem);
							viewPaths = SCommonFuction::ClipPaths(viewPaths, item->vecBoundingPolygon[i]);
					}
				}
			}
		}
		SB = SB - 1;
		if (SB < 0)
			return S;
	}
	return S; 
}


