#include "stdafx.h"
#include "Inventory.h"

void Inventory::SetDataCoverage(std::vector<S100::DataCoverage> dataCoverages)
{
	vecDataCoverage = dataCoverages;
}

std::vector<S100::DataCoverage> Inventory::GetDataCoverage()
{
	return vecDataCoverage;
}
