#include "stdafx.h"
#include "CatalogManager.h"
#include "GISLibrary.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <algorithm>

CatalogManager::CatalogManager()
{

}

CatalogManager::~CatalogManager()
{
	for (auto i = FCs.begin(); i != FCs.end(); i++)
	{
		delete (*i);
	}

	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		delete (*i);
	}
}

FeatureCatalogue* CatalogManager::addFC(std::string path)
{
	return addFC(LibMFCUtil::StringToWString(path));
}

FeatureCatalogue* CatalogManager::addFC(std::wstring path)
{
	auto fc = new FeatureCatalogue();

	if (fc->Read(path))
	{
		FCs.push_back(fc);
		return fc;
	}
	
	delete fc;
	return nullptr;
}

void CatalogManager::addFC(FeatureCatalogue* fc)
{
	if (fc) {
		FCs.push_back(fc);
	}
}

PortrayalCatalogue* CatalogManager::addPC(std::string path)
{
	return addPC(LibMFCUtil::StringToWString(path));
}

PortrayalCatalogue* CatalogManager::addPC(std::wstring path)
{
	auto pc = new PortrayalCatalogue();

	if (pc->Open(path)) {
		if (pc) {
			pc->CreateSVGD2Geometry(gisLib->D2.pD2Factory);
			pc->CreatePatternImages(gisLib->D2.pD2Factory, 
				gisLib->D2.pImagingFactory, 
				gisLib->D2.D2D1StrokeStyleGroup.at(0));
			pc->CreateLineImages(gisLib->D2.pD2Factory, 
				gisLib->D2.pImagingFactory, 
				gisLib->D2.D2D1StrokeStyleGroup.at(0));
		}

		PCs.push_back(pc);
		return pc;
	}

	delete pc;
	return nullptr;
}

void CatalogManager::addPC(PortrayalCatalogue* pc)
{
	if (pc) {
		PCs.push_back(pc);
	}
}

FeatureCatalogue* CatalogManager::getFC(std::string productId) const
{
	std::vector<FeatureCatalogue*> fc;

	for (auto i = FCs.begin(); i != FCs.end(); i++)
	{
		if (productId.compare((*i)->getProductId()) == 0)
		{
			fc.push_back((*i));
		}
	}

	std::sort(fc.begin(), fc.end(), [](FeatureCatalogue* a, FeatureCatalogue* b) {
		return a->getVersion() < b->getVersion();
		});

	if (fc.size() > 0)
	{
		return fc.back();
	}

	return nullptr;
}

PortrayalCatalogue* CatalogManager::getPC(std::string productId) const
{
	std::vector<PortrayalCatalogue*> pc;

	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		if (productId.compare((*i)->getProduct()) == 0)
		{
			pc.push_back((*i));
		}
	}

	std::sort(pc.begin(), pc.end(), [](PortrayalCatalogue* a, PortrayalCatalogue* b) {
		return a->getVersion() < b->getVersion();
		});

	if (pc.size() > 0)
	{
		return pc.back();
	}

	// Failed to find PC, return S-100 PC
	return getPC("S-100");

	//return nullptr;
}

FeatureCatalogue* CatalogManager::getFC(int productId) const
{
	auto product = "S-" + std::to_string(productId);
	return getFC(product);
}

PortrayalCatalogue* CatalogManager::getPC(int productId) const
{
	auto product = "S-" + std::to_string(productId);
	return getPC(product);
}

void CatalogManager::ChangeColorPallete(std::string paletteName, ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		(*i)->ChangePalette(paletteName, d2Factory, imageFactory, stroke);
	}
}

void CatalogManager::ChangeColorPallete(std::wstring paletteName, ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	ChangeColorPallete(pugi::as_utf8(paletteName), d2Factory, imageFactory, stroke);
}

const std::set<std::string> CatalogManager::getFcProductList()
{
	std::set<std::string> result;

	for (auto i = FCs.begin(); i != FCs.end(); i++) {
		auto productId = (*i)->getProductId();
		result.insert(productId);
	}

	return result;
}

const std::set<std::string> CatalogManager::getPcProductList()
{
	std::set<std::string> result;

	for (auto i = PCs.begin(); i != PCs.end(); i++) {
		auto productId = (*i)->getProduct();
		result.insert(productId);
	}

	return result;
}