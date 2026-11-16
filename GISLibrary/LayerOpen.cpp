#include "stdafx.h"
// OpenLayer: picks the dataset class of a background or exchange-catalogue layer. Kept apart
// from Layer.cpp because the exchange-catalogue view draws with Direct2D.
#include "PathUtil.h"
#include "TextCompare.h"
#include "LayerOpen.h"
#include "Layer.h"
#include "S100Layer.h"
#include "S100ExchangeCatalogue.h"

bool GISLibrary::OpenLayer(Layer& layer, const std::wstring& _filepath, LayerManager* lm)
{
	if (auto s100Layer = dynamic_cast<S100Layer*>(&layer))
	{
		return OpenLayer(*s100Layer, _filepath, lm);
	}

	const auto extension = s100::path::Extension(_filepath);

	if (nullptr == layer.m_spatialObject)
	{
		if (s100::text::IEquals(extension, L"SHP")) {
			layer.m_spatialObject = new SHPFile();
		}
		else if (s100::text::IEquals(extension, L"XML")) {
			layer.m_spatialObject = new S100ExchangeCatalogue(lm->GetScaler(), lm->catalogManager, lm->GetRenderResources(), lm);
		}
	}

	if (nullptr == layer.m_spatialObject)
	{
		return false;
	}
	layer.m_spatialObject->m_pLayer = &layer;

	// Relative paths are resolved against the executable directory (legacy behaviour).
	const std::wstring fullPath = s100::path::IsAbsolute(_filepath)
		? _filepath
		: (s100::path::ExecutableDirectory() / _filepath).wstring();

	return layer.m_spatialObject->Open(fullPath);
}
