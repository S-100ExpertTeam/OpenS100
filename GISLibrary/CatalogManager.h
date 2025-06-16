#pragma once

#include <vector>

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

class CatalogManager
{
public:
	CatalogManager(D2D1Resources* d2d1);
	virtual ~CatalogManager();

public:
	std::vector<FeatureCatalogue*> FCs;
	std::vector<PortrayalCatalogue*> PCs;
	D2D1Resources* D2;

public:
	FeatureCatalogue* addFC(std::string path);
	FeatureCatalogue* addFC(std::wstring path);
	void addFC(FeatureCatalogue* fc);
	PortrayalCatalogue* addPC(std::string path);
	PortrayalCatalogue* addPC(std::wstring path);
	void addPC(PortrayalCatalogue* pc);

	FeatureCatalogue* getFC(std::string productId = "S-101") const;
	PortrayalCatalogue* getPC(std::string productId = "S-101") const;

	FeatureCatalogue* getFC(int productId) const;
	PortrayalCatalogue* getPC(int productId) const;

	void ChangeColorPallete(std::string paletteName, ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);
	void ChangeColorPallete(std::wstring paletteName, ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);

	const std::set<std::string> getFcProductList();
	const std::set<std::string> getPcProductList();
};