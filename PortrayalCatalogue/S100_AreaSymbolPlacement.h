#pragma once

#include "S100_AreaPlacementMode.h"

#include <pugixml.hpp>

class S100_AreaSymbolPlacement
{
public:
	S100_AreaSymbolPlacement() = default;
	virtual ~S100_AreaSymbolPlacement() = default;

private:
	S100_AreaPlacementMode placementMode;

public:
	S100_AreaPlacementMode GetPlacementMode() const;
	void SetPlacementMode(const S100_AreaPlacementMode mode);

	void GetContents(pugi::xml_node& node);
};

