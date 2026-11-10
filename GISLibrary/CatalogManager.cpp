#include "stdafx.h"
#include "../GeoMetryLibrary/TextCodec.h"
#include "CatalogManager.h"
#include "GISLibrary.h"

#include "../SystemDatabase/ResourceStore.h"
#include "../SystemDatabase/SystemDatabase.h"

#include <algorithm>
#include <memory>

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
	return addFC(s100::text::ToWide(path));
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
	return addPC(s100::text::ToWide(path));
}

PortrayalCatalogue* CatalogManager::addPC(std::wstring path)
{
	auto pc = new PortrayalCatalogue();

	if (pc->Open(path)) {
		// Render caches (SVG geometry, pattern bitmaps) are built by the renderer:
		// GISLibrary::PortrayalRender::Prepare(pc, d2).
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

FeatureCatalogue* CatalogManager::getFC(std::string productId)
{
	loadNewestRegisteredFC(productId);

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

FeatureCatalogue* CatalogManager::getFC(std::string productId, Version version) const
{
	for (auto i = FCs.begin(); i != FCs.end(); i++)
	{
		if (productId.compare((*i)->getProductId()) == 0 && version == (*i)->getVersion())
		{
			return *i;
		}
	}

	//Version currentVersion = version;
	//
	//while (currentVersion.isEmpty() == false)
	//{
	//	for (auto i = FCs.begin(); i != FCs.end(); i++)
	//	{
	//		if (productId.compare((*i)->getProductId()) == 0 && version == (*i)->getVersion())
	//		{
	//			return *i;
	//		}
	//	}

	//	currentVersion = currentVersion.getReducedVersion();
	//}

	return nullptr;
}

PortrayalCatalogue* CatalogManager::getPC(std::string productId, Version version) const
{
	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		if (productId.compare((*i)->getProduct()) == 0 && version == (*i)->getVersion())
		{
			return *i;
		}
	}

	//Version currentVersion = version;

	//while (currentVersion.isEmpty() == false)
	//{
	//	for (auto i = PCs.begin(); i != PCs.end(); i++)
	//	{
	//		if (productId.compare((*i)->getProduct()) == 0 && version == (*i)->getVersion())
	//		{
	//			return *i;
	//		}
	//	}

	//	currentVersion = currentVersion.getReducedVersion();
	//}
	//
	return nullptr;
}

PortrayalCatalogue* CatalogManager::getPC(std::string productId)
{
	loadNewestRegisteredPC(productId);

	PortrayalCatalogue* bpc = nullptr;
	std::vector<PortrayalCatalogue*> pc;

	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		if (productId.compare((*i)->getProduct()) == 0)
		{
			pc.push_back((*i));
		}
		else if ((*i)->getProduct().compare("S-100") == 0)
		{
			bpc = (*i);
		}
	}

	std::sort(pc.begin(), pc.end(), [](PortrayalCatalogue* a, PortrayalCatalogue* b) {
		return a->getVersion() < b->getVersion();
		});

	if (pc.size() > 0)
	{
		return pc.back();
	}

	if (bpc)
	{
		return bpc;
	}

	return nullptr;
}

FeatureCatalogue* CatalogManager::getFC(int productId)
{
	auto product = "S-" + std::to_string(productId);
	return getFC(product);
}

FeatureCatalogue* CatalogManager::getFC(int productId, Version version) const
{
	auto product = "S-" + std::to_string(productId);
	return getFC(product, version);
}

PortrayalCatalogue* CatalogManager::getPC(int productId)
{
	auto product = "S-" + std::to_string(productId);
	return getPC(product);
}

PortrayalCatalogue* CatalogManager::getPC(int productId, Version version) const
{
	auto product = "S-" + std::to_string(productId);
	return getPC(product, version);
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

// --- installed catalogues, read on first use -----------------------------------------------

std::size_t CatalogManager::indexOf(const std::vector<s100::sdb::CatalogueRecord>& records, const s100::sdb::CatalogueRecord& record)
{
	const auto found = std::find_if(records.begin(), records.end(), [&](const s100::sdb::CatalogueRecord& known) {
		return s100::sdb::productFromIdentifier(known.product) == s100::sdb::productFromIdentifier(record.product)
			&& s100::sdb::compareVersions(known.version, record.version) == 0;
	});
	return static_cast<std::size_t>(found - records.begin());
}

void CatalogManager::registerFeatureCatalogue(const s100::sdb::CatalogueRecord& record, const std::filesystem::path& file)
{
	const std::size_t index = indexOf(registeredFCRecords, record);
	if (index < registeredFCRecords.size())
	{
		registeredFCRecords[index] = record;
		registeredFCs[index] = Registration{ file };
		return;
	}
	registeredFCRecords.push_back(record);
	registeredFCs.push_back(Registration{ file });
}

void CatalogManager::registerPortrayalCatalogue(const s100::sdb::CatalogueRecord& record, const std::filesystem::path& file)
{
	const std::size_t index = indexOf(registeredPCRecords, record);
	if (index < registeredPCRecords.size())
	{
		registeredPCRecords[index] = record;
		registeredPCs[index] = Registration{ file };
		return;
	}
	registeredPCRecords.push_back(record);
	registeredPCs.push_back(Registration{ file });
}

void CatalogManager::registerInstalled(const s100::sdb::SystemDatabase& database, const s100::sdb::ResourceStore& store)
{
	for (const auto& record : database.featureCatalogues().all())
	{
		registerFeatureCatalogue(record, store.absolute(record.path));
	}
	for (const auto& record : database.portrayalCatalogues().all())
	{
		registerPortrayalCatalogue(record, store.absolute(record.path));
	}
}

void CatalogManager::clearRegistrations()
{
	registeredFCRecords.clear();
	registeredFCs.clear();
	registeredPCRecords.clear();
	registeredPCs.clear();
}

FeatureCatalogue* CatalogManager::loadRegisteredFC(std::size_t index)
{
	Registration& registration = registeredFCs[index];
	if (!registration.loaded && !registration.failed)
	{
		auto fc = std::make_unique<FeatureCatalogue>();
		if (fc->Read(registration.file.wstring()))
		{
			registration.loaded = fc.get();
			FCs.push_back(fc.release());
		}
		else
		{
			registration.failed = true;
		}
	}
	return static_cast<FeatureCatalogue*>(registration.loaded);
}

PortrayalCatalogue* CatalogManager::loadRegisteredPC(std::size_t index)
{
	Registration& registration = registeredPCs[index];
	if (!registration.loaded && !registration.failed)
	{
		auto pc = std::make_unique<PortrayalCatalogue>();
		if (pc->Open(registration.file.wstring()))
		{
			registration.loaded = pc.get();
			PCs.push_back(pc.release());
			if (onPortrayalLoaded)
			{
				onPortrayalLoaded(static_cast<PortrayalCatalogue*>(registration.loaded));
			}
		}
		else
		{
			registration.failed = true;
		}
	}
	return static_cast<PortrayalCatalogue*>(registration.loaded);
}

void CatalogManager::loadNewestRegisteredFC(std::string_view product)
{
	const auto resolution = s100::sdb::resolveCatalogue(product, "", registeredFCRecords);
	if (resolution.catalogue)
	{
		(void)loadRegisteredFC(indexOf(registeredFCRecords, *resolution.catalogue));
	}
}

void CatalogManager::loadNewestRegisteredPC(std::string_view product)
{
	const auto resolution = s100::sdb::resolveCatalogue(product, "", registeredPCRecords);
	if (resolution.catalogue)
	{
		(void)loadRegisteredPC(indexOf(registeredPCRecords, *resolution.catalogue));
	}
}

CatalogManager::Resolution CatalogManager::resolve(std::string_view product, std::string_view version)
{
	Resolution resolution;
	resolution.feature = s100::sdb::resolveCatalogue(product, version, registeredFCRecords);
	if (resolution.feature.catalogue)
	{
		resolution.fc = loadRegisteredFC(indexOf(registeredFCRecords, *resolution.feature.catalogue));
		if (!resolution.fc)
		{
			resolution.feature.match = s100::sdb::CatalogueMatch::Unresolved;
		}
	}
	resolution.portrayal = s100::sdb::resolveCatalogue(product, version, registeredPCRecords);
	if (resolution.portrayal.catalogue)
	{
		resolution.pc = loadRegisteredPC(indexOf(registeredPCRecords, *resolution.portrayal.catalogue));
		if (!resolution.pc)
		{
			resolution.portrayal.match = s100::sdb::CatalogueMatch::Unresolved;
		}
	}
	return resolution;
}
