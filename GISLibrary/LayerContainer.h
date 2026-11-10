#pragma once

class LayerManager;

namespace GISLibrary
{
	// A spatial object that draws layers of its own: an exchange set viewed without installing it,
	// the System Database chart. Settings that change how S-100 layers are portrayed (palette,
	// safety contour, ...) reach these layers through it.
	class LayerContainer
	{
	public:
		virtual ~LayerContainer() = default;

		// The layers drawn now; null when there are none yet.
		[[nodiscard]] virtual LayerManager* GetInnerLayers() = 0;
		// Forgets layers kept for later (not drawn now), which would come back with the old settings.
		virtual void DropCachedLayers() = 0;
	};
}
