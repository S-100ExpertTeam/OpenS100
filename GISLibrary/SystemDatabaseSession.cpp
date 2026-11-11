#include "stdafx.h"
#include "SystemDatabaseSession.h"

#include "CatalogManager.h"
#include "Layer.h"
#include "LayerManager.h"

#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <shlobj.h>
#endif

namespace GISLibrary
{
	SystemDatabaseSession::SystemDatabaseSession(std::filesystem::path root)
		: store_(std::move(root)),
		  database_(s100::sdb::SystemDatabase::open(store_.databaseFile())),
		  importer_(database_, store_, s100::sdb::Importer::Readers{ { &datasetInspector_, &gridInspector_, &gmlInspector_ }, &catalogueInspector_, &exchangeSetReader_ })
	{
	}

	SystemDatabaseSession::~SystemDatabaseSession()
	{
		detach();
	}

	std::filesystem::path SystemDatabaseSession::defaultRoot()
	{
#ifdef _WIN32
		wchar_t* overridden = nullptr;
		std::size_t length = 0;
		if (_wdupenv_s(&overridden, &length, L"OPENS100_SDB_ROOT") == 0 && overridden && *overridden)
		{
			std::filesystem::path root(overridden);
			std::free(overridden);
			return root;
		}
		std::free(overridden);

		PWSTR folder = nullptr;
		std::filesystem::path base;
		if (SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &folder)))
		{
			base = folder;
		}
		::CoTaskMemFree(folder);
		if (base.empty())
		{
			base = std::filesystem::temp_directory_path();
		}
		return base / L"OpenS100" / L"SDB";
#else
		if (const char* overridden = std::getenv("OPENS100_SDB_ROOT"); overridden && *overridden)
		{
			return overridden;
		}
		if (const char* data = std::getenv("XDG_DATA_HOME"); data && *data)
		{
			return std::filesystem::path(data) / "opens100" / "sdb";
		}
		const char* home = std::getenv("HOME");
		return std::filesystem::path(home ? home : ".") / ".local" / "share" / "opens100" / "sdb";
#endif
	}

	void SystemDatabaseSession::attach(LayerManager& layers, CatalogManager& catalogues, DatasetLayerSet::CacheLimit cache)
	{
		detach();
		layers_ = &layers;
		catalogues_ = &catalogues;
		catalogues_->registerInstalled(database_, store_);

		cache_ = std::move(cache);
		addChart();
	}

	void SystemDatabaseSession::addChart()
	{
		auto layer = SystemDatabaseChart::CreateLayer(*layers_, database_, store_, cache_);
		chart_ = static_cast<SystemDatabaseChart*>(layer->m_spatialObject);
		chartLayer_ = layer.release();
		layers_->AddLayer(chartLayer_);
	}

	SystemDatabaseChart* SystemDatabaseSession::chart() const
	{
		if (!layers_ || !chartLayer_ || std::find(layers_->layers.begin(), layers_->layers.end(), chartLayer_) == layers_->layers.end())
		{
			return nullptr;
		}
		return chart_;
	}

	void SystemDatabaseSession::detach()
	{
		if (chart())
		{
			(void)layers_->ReleaseLayer(chartLayer_);   // deletes the layer and the chart
		}
		chart_ = nullptr;
		chartLayer_ = nullptr;
		layers_ = nullptr;
		catalogues_ = nullptr;
	}

	void SystemDatabaseSession::refreshCatalogues()
	{
		if (catalogues_)
		{
			catalogues_->registerInstalled(database_, store_);
		}
		if (auto* attached = chart())
		{
			attached->CataloguesChanged();
		}
	}

	void SystemDatabaseSession::refreshDatasets()
	{
		if (auto* attached = chart())
		{
			attached->Reload();
		}
		else if (layers_)
		{
			addChart();   // its layer was deleted from the layer list; datasets were installed since
		}
	}
}
