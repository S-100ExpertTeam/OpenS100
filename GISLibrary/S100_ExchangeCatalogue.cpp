#include "stdafx.h"
#include "S100_ExchangeCatalogue.h"

int S100::S100_ExchangeCatalogue::getCountDatasetDiscoveryMetadata()
{
    return DatasetDiscoveryMetadata.size();
}

std::string S100::S100_ExchangeCatalogue::getDatasetPath(int index)
{
    return std::string();
}
