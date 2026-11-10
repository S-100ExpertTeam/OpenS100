#pragma once

#include "../SystemDatabase/DatasetSelection.h"

#include <optional>

class Scaler;

namespace GISLibrary
{
	// The S-98 E-1.3 selection input for what the scaler shows: its display scale, its device size,
	// longitude/latitude to device through the viewer's Mercator projection, and the ground the
	// view covers (left out while the map is rotated, when the map box no longer bounds the view).
	// std::nullopt while the scaler has no screen or scale yet. The projection refers to `scaler`,
	// so the view must not outlive it.
	[[nodiscard]] std::optional<s100::sdb::DatasetSelector::View> selectionView(Scaler& scaler);
}
