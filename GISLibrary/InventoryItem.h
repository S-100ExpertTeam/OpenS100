#pragma once

#include "ScaleBand.h"

#include <string>
#include <vector>
#include <memory>

class InventoryItem {
public:
	//EX : 10100AA_X0000.000
	std::string strFileName;
	std::shared_ptr<MBR> mbrBoundingBox;
	std::vector<D2D1_POINT_2F> BoundingPolygon;
	ScaleBand ScaleRange;

	CString strFilePath;
};