#pragma once

#include <memory>

namespace GISLibrary
{
	// What a renderer keeps next to a model object so that it does not rebuild it every frame
	// (a measured text size, a built path). The model does not know what is inside: it owns the
	// slot and destroys it with the object, which is all the invalidation the cache needs. Each
	// renderer derives its own entries (D2DGeometryCache; the text size of DisplayListPainter).
	struct RenderCacheEntry
	{
		virtual ~RenderCacheEntry() = default;
	};

	using RenderCacheSlot = std::unique_ptr<RenderCacheEntry>;
}
