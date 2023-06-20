#pragma once

#include <vector>

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

class CatalogManager
{
public:
	CatalogManager();
	virtual ~CatalogManager();

public:
	std::vector<FeatureCatalogue*> FCs;
	std::vector<PortrayalCatalogue*> PCs;

public:
	FeatureCatalogue* AddFC(std::string path);
	PortrayalCatalogue* AddPC(std::string path);
};