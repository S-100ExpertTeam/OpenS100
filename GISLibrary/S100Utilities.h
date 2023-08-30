#pragma once

#include "ScaleBands.h"
#include "DataCoverage.h"
#include "Inventory.h"
#include "../GeoMetryLibrary/MBR.h"

#include <string>
#include <vector>

class S100Utilities
{
public:
	static int GetLevel(std::wstring path);

	static ScaleBands scaleBands[15];

	static int GetScaleBand(int scale);
	static std::vector<int> GetScaleBands(DataCoverage dataCoverage);
	static std::vector<Inventory> SelectDataCoverages(std::vector<Inventory>, int scale, MBR viewport);
};