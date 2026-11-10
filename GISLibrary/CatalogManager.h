#pragma once

#include <filesystem>
#include <functional>
#include <string_view>
#include <vector>

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../SystemDatabase/CatalogueResolution.h"

namespace s100::sdb
{
	class ResourceStore;
	class SystemDatabase;
}

class CatalogManager
{
public:
	CatalogManager();
	virtual ~CatalogManager();

public:
	// Loaded catalogues, owned by this object.
	std::vector<FeatureCatalogue*> FCs;
	std::vector<PortrayalCatalogue*> PCs;

	// Called once for every portrayal catalogue this object loads from a registration, so the
	// renderer can build its caches (GISLibrary::PortrayalRender::Prepare). Not called by addPC.
	std::function<void(PortrayalCatalogue*)> onPortrayalLoaded;

public:
	FeatureCatalogue* addFC(std::string path);
	FeatureCatalogue* addFC(std::wstring path);
	void addFC(FeatureCatalogue* fc);
	PortrayalCatalogue* addPC(std::string path);
	PortrayalCatalogue* addPC(std::wstring path);
	void addPC(PortrayalCatalogue* pc);

	// The newest catalogue of the product, loading the newest registered one first if it is newer
	// than any loaded.
	FeatureCatalogue* getFC(std::string productId = "S-101");
	PortrayalCatalogue* getPC(std::string productId = "S-101");

	FeatureCatalogue* getFC(std::string productId, Version version) const;
	PortrayalCatalogue* getPC(std::string productId, Version version) const;

	FeatureCatalogue* getFC(int productId);
	FeatureCatalogue* getFC(int productId, Version version) const;
	PortrayalCatalogue* getPC(int productId);
	PortrayalCatalogue* getPC(int productId, Version version) const;


	const std::set<std::string> getFcProductList();
	const std::set<std::string> getPcProductList();

	// --- installed catalogues, read on first use -------------------------------------------
	// Registering reads nothing: a catalogue is read the first time a dataset needs it, so
	// start-up costs no parsing. Registering the same product and version again replaces the
	// registration (a catalogue already loaded from it stays loaded).
	void registerFeatureCatalogue(const s100::sdb::CatalogueRecord& record, const std::filesystem::path& file);
	void registerPortrayalCatalogue(const s100::sdb::CatalogueRecord& record, const std::filesystem::path& file);
	// Makes the registrations those of the catalogues installed in the System Database: new ones
	// are added, removed ones dropped, unchanged ones keep what was loaded from them.
	void registerInstalled(const s100::sdb::SystemDatabase& database, const s100::sdb::ResourceStore& store);
	void clearRegistrations();

	[[nodiscard]] const std::vector<s100::sdb::CatalogueRecord>& registeredFeatureCatalogues() const noexcept { return registeredFCRecords; }
	[[nodiscard]] const std::vector<s100::sdb::CatalogueRecord>& registeredPortrayalCatalogues() const noexcept { return registeredPCRecords; }

	// The catalogues that portray a dataset of `product` following edition `version` of its
	// specification (empty when the dataset does not say), by the rules of
	// s100::sdb::resolveCatalogue, loading them if needed. A catalogue that fails to load is
	// reported as unresolved and not tried again.
	struct Resolution
	{
		FeatureCatalogue* fc = nullptr;
		PortrayalCatalogue* pc = nullptr;
		s100::sdb::CatalogueResolution feature;
		s100::sdb::CatalogueResolution portrayal;
	};
	Resolution resolve(std::string_view product, std::string_view version);

private:
	struct Registration
	{
		std::filesystem::path file;
		void* loaded = nullptr;   // FeatureCatalogue* or PortrayalCatalogue*, owned by FCs/PCs
		bool failed = false;
	};

	FeatureCatalogue* loadRegisteredFC(std::size_t index);
	PortrayalCatalogue* loadRegisteredPC(std::size_t index);
	void loadNewestRegisteredFC(std::string_view product);
	void loadNewestRegisteredPC(std::string_view product);
	static std::size_t indexOf(const std::vector<s100::sdb::CatalogueRecord>& records, const s100::sdb::CatalogueRecord& record);

	// Parallel vectors: the records feed s100::sdb::resolveCatalogue as they are.
	std::vector<s100::sdb::CatalogueRecord> registeredFCRecords;
	std::vector<Registration> registeredFCs;
	std::vector<s100::sdb::CatalogueRecord> registeredPCRecords;
	std::vector<Registration> registeredPCs;
};
