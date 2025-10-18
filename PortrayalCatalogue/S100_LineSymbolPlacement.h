#pragma once

#include "S100_LinePlacementMode.h"

#include <optional>

#include <pugixml.hpp>

class S100_LineSymbolPlacement
{
public:
	S100_LineSymbolPlacement() = default;
	virtual ~S100_LineSymbolPlacement() = default;

private:
	double offset = 0.0;
	S100_LinePlacementMode placementMode = S100_LinePlacementMode::none;
	std::optional<bool> visibleParts;

public:
	double GetOffset() const;
	void SetOffset(double value);

	S100_LinePlacementMode GetPlacementMode() const;
	void SetPlacementMode(S100_LinePlacementMode value);

	std::optional<bool> GetVisibleParts() const;
	void SetVisibleParts(bool value);

	// Read contents from XML node using pugixml
	void GetContents(pugi::xml_node& node);
};

