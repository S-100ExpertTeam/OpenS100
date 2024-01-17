#pragma once

#include "ScaleBands.h"
#include "DataCoverage.h"
#include "../GeoMetryLibrary/MBR.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <string>
#include <vector>
#include <memory>
#include <polyclipping/clipper.hpp>
#include <algorithm> 


class ScaleBand {
public:
	int MaxDisplayScale;
	int MinDisplayScale;

	bool isOverlap(int scale)
	{
		return MaxDisplayScale < scale&& scale < MinDisplayScale ? true : false;
	}

	bool isIntersection(int minimumScale, int maximumScale)
	{
		if (minimumScale == -1)
		{
			if (MinDisplayScale >= maximumScale || MaxDisplayScale >= maximumScale)
				return true;
		}

		if ((minimumScale >= MinDisplayScale && MinDisplayScale >= maximumScale) || (minimumScale >= MaxDisplayScale && MaxDisplayScale >= maximumScale))
		{
			return true;
		}
		else
			return false;
	}

	static bool CompareByScale(const ScaleBand& a, const ScaleBand& b) {
		if (a.MinDisplayScale == b.MinDisplayScale) {
			return a.MaxDisplayScale > b.MaxDisplayScale;
		}
		return a.MinDisplayScale > b.MinDisplayScale;
	}
};

class Inventory {
public:
	//EX : 10100AA_X0000.000
	std::string strFileName;
	MBR mbrBoundingBox;
	//std::vector<SSurface*> vecBoundingPolygon;
	std::vector< std::vector<D2D1_POINT_2F>> vecBoundingPolygon;
	std::vector<ScaleBand> vecScaleRange; 

	ScaleBand totalScaleBand;
	CString strFilePath;
};


class InventoryItem {
public:
	//EX : 10100AA_X0000.000
	std::string strFileName;
	MBR mbrBoundingBox;
	std::vector<D2D1_POINT_2F> BoundingPolygon;
	ScaleBand ScaleRange;

	CString strFilePath;
};


class S100Utilities
{
public:
	static int GetLevel(std::wstring path);

	static ScaleBands scaleBands[15];

	D2D1::ColorF GetColorNum(int num);

	static int GetScaleBand(int scale);
	static std::vector<int> GetScaleBands(S100::DataCoverage dataCoverage);
	static std::vector<int> GetScaleBands(ScaleBand sb);
	static std::vector<std::shared_ptr<InventoryItem>> SelectDataCoverages(std::vector<std::shared_ptr<Inventory>> INV, Scaler* scaler, MBR viewport);

};