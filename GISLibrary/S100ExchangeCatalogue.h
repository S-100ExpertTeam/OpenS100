#pragma once
#include "DeviceTypes.h"
#include <string>
#include "SpatialObject.h"
#include "ExchangeCatalogue.h"

#include "DatasetLayerSet.h"
#include "LayerContainer.h"

#include "../SystemDatabase/DatasetSelection.h"

#include "IRenderer.h"

#include <memory>
#include <span>
#include <utility>

using namespace S100;

class Geometry;
class Scaler;
class Inventory;
class InventoryItem;
class ScaleRange;


class S100ExchangeCatalogue :
	public SpatialObject, public GISLibrary::LayerContainer
{
public:
	S100ExchangeCatalogue(Scaler* scaler, CatalogManager* cm, LayerManager* parent);
	virtual ~S100ExchangeCatalogue();

public:
	//unsigned int m_nRecords;					// The number of records
	//unsigned int m_nShapeType;					// Shape Type
	std::shared_ptr<ExchangeCatalogue>  m_DataPtr;	//

	std::vector<std::shared_ptr<Inventory>> m_vecInventory;

	LayerManager* m_pLayerManager;

	LayerManager* m_pParentLayerManager;


public:
	/*
	** Description: Read the file and store the data in the layer.
	*/
	bool Open(const std::wstring& _filepath) override;
	bool OpenMetadata(const std::wstring& _filepath) override;

	void Draw(GISLibrary::ViewFrame& frame, Scaler* scaler, double offset) override;

	void DrawFiles(Scaler* scaler, std::vector<std::shared_ptr<InventoryItem>>& item);
	// The boxes of the datasets shown, their file names and scales, in device pixels.
	void DrawOverlay(s100::render::IRenderer& renderer, Scaler& scaler, const std::vector<std::shared_ptr<InventoryItem>>& items);
	
	std::wstring FixFileName(const std::string& str);

	std::wstring FixLegacyFileName(const std::string& str);

	std::wstring GetFirstLayerFilePath();

	LayerManager* GetInnerLayers() override { return m_pLayerManager; }
	void DropCachedLayers() override { if (m_layers) m_layers->dropCache(); }
	bool ReloadLayers() override { return false; }

	// The layers of the selected datasets and the cache of deselected ones; null before Open.
	[[nodiscard]] const GISLibrary::DatasetLayerSet* GetLayerSet() const noexcept { return m_layers.get(); }

private:
	// One item per dataset of a selection, the larger scale first.
	std::vector<std::shared_ptr<InventoryItem>> ItemsOf(std::span<const s100::sdb::SelectedCoverage> selection) const;

	// S-98 E-1.3 selection over the catalogue's S100XC:dataCoverage. A dataset is identified by
	// its index in m_vecInventory.
	s100::sdb::DatasetSelector m_selector;
	// The selected datasets' layers in m_pLayerManager, and the recently deselected ones.
	std::unique_ptr<GISLibrary::DatasetLayerSet> m_layers;
	// For each coverage of m_selector, in the same order: its index in m_vecInventory and its
	// index among that entry's data coverages.
	std::vector<std::pair<std::size_t, std::size_t>> m_coverageOwners;
};