#pragma once
#include <string>

class Layer;
class LayerManager;
class S100Layer;

namespace GISLibrary
{
	// Opens the dataset at `path` into `layer` for display: an S-100 dataset (ISO 8211, GML,
	// HDF5) for an S100Layer, a shapefile or exchange catalogue otherwise. The dataset classes
	// that draw themselves are chosen here, which is why this is not a member of Layer.
	bool OpenLayer(Layer& layer, const std::wstring& path, LayerManager* manager);
	bool OpenLayer(S100Layer& layer, const std::wstring& path, LayerManager* manager);
}
