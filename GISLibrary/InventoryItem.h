#pragma once

#include "ScaleBand.h"
#include "DrawTypes.h"

#include <string>
#include <vector>
#include <memory>

class InventoryItem {
public:
	//EX : 10100AA_X0000.000
	std::string strFileName;
	std::shared_ptr<MBR> mbrBoundingBox;
	std::vector<GISLibrary::PointF> BoundingPolygon;
	ScaleBand ScaleRange;

	std::wstring strFilePath;
};