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
	FeatureCatalogue* addFC(std::string path);
	FeatureCatalogue* addFC(std::wstring path);
	void addFC(FeatureCatalogue* fc);
	PortrayalCatalogue* addPC(std::string path);
	PortrayalCatalogue* addPC(std::wstring path);
	void addPC(PortrayalCatalogue* pc);

	FeatureCatalogue* getFC(std::string productId = "S-101") const;
	PortrayalCatalogue* getPC(std::string productId = "S-101") const;

	FeatureCatalogue* getFC(std::string productId, Version version) const;
	PortrayalCatalogue* getPC(std::string productId, Version version) const;

	FeatureCatalogue* getFC(int productId) const;
	FeatureCatalogue* getFC(int productId, Version version) const;
	PortrayalCatalogue* getPC(int productId) const;
	PortrayalCatalogue* getPC(int productId, Version version) const;


	const std::set<std::string> getFcProductList();
	const std::set<std::string> getPcProductList();
};