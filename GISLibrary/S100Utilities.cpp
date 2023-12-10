#include "stdafx.h"
#include "S100Utilities.h"

#include <string>
#include <filesystem>
#include <cctype>

ScaleBands S100UtilitiesscaleBands[15] = {
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
	if (scale < scaleBands[0].maximumScale) {
		return 1;
	}

	for (int i = 1; i <= 14; i++) {
		if (scaleBands[i].minimumScale <= scale && scale < scaleBands[i].maximumScale) {
			return i;
		}
	}

	return 15;
}
//
//std::vector<int> S100Utilities::GetScaleBands(DataCoverage dataCoverage)
//{
//	int minDS = dataCoverage.getMinDS();
//	int maxDS = dataCoverage.getMaxDS();
//	std::vector<int> S;
//
//	if (minDS < scaleBands[0].maximumScale) {
//		S.push_back(0);
//	}
//
//	for (int i = 1; i <= 14; i++) {
//		if (max(minDS, scaleBands[i].minimumScale) < min(maxDS, scaleBands[i].maximumScale)) {
//			S.push_back(i);
//		}
//	}
//
//	return S;
//}
//
//std::vector<Inventory> S100Utilities::SelectDataCoverages(std::vector<Inventory>, int scale, MBR viewport)
//{
//	std::vector<Inventory> S;
//
//	int SB = GetScaleBand(scale);
//
//	
//
//	return S;
//}