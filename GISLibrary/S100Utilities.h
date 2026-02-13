#pragma once

#include "ScaleBand.h"
#include "DataCoverage.h"
#include "Inventory.h"
#include "InventoryItem.h"

#include "../GeoMetryLibrary/MBR.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <string>
#include <vector>
#include <memory>
#include <polyclipping/clipper.hpp>
#include <algorithm>

class S100Utilities
{
public:
	static int GetLevel(std::wstring path);

	static ScaleBand scaleBand[15];

	D2D1::ColorF GetColorNum(int num);

	static int GetScaleBand(int scale);
	static std::vector<int> algorithm_ScaleBand(S100::DataCoverage dataCoverage);
	static std::vector<int> GetScaleBand(ScaleBand sb);
	static std::vector<std::shared_ptr<InventoryItem>> SelectDataCoverages(std::vector<std::shared_ptr<Inventory>> INV, Scaler* scaler, MBR viewport);

	ScaleBand GetLegacyScaleband(int scale);
};