#include "stdafx.h"
#include "S100Utilities.h"
#include "GISLibrary.h"

#include "../LatLonUtility/cpp_util.h"

#include <algorithm>
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
	std::wstring name = std::filesystem::path(path).stem().wstring();
	if (name.length() >= 8)
	{
		auto levelStr = name.substr(7, 1);
		if (std::isdigit(levelStr.at(0)))
		{
			auto level = cpp_util::stoi(levelStr);
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
		if ((std::min)(minDS, scaleBand[i].GetMinimumDisplayScale()) > (std::max)(optDS, scaleBand[i].GetOptimumDisplayScale())) {
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
		if ((std::max)(maxDS, scaleBand[i].GetOptimumDisplayScale()) < (std::min)(minDS, scaleBand[i].GetMinimumDisplayScale())) {
			S.push_back(i);
		}
	}
	return S;
}
