#pragma once

#include "ScaleBand.h"
#include "DataCoverage.h"

#include <string>
#include <memory>
#include <vector>

class Inventory {
public:
	//EX : 10100AA_X0000.000
	std::string strFileName;
	std::shared_ptr<MBR> mbrBoundingBox;
	//std::vector<SSurface*> vecBoundingPolygon;
	std::vector<std::vector<D2D1_POINT_2F>> vecBoundingPolygon;
	std::vector<ScaleBand> vecScaleRange;

	ScaleBand totalScaleBand;
	CString strFilePath;

private:
	std::vector<S100::DataCoverage> vecDataCoverage;

public:
	void SetDataCoverage(std::vector<S100::DataCoverage> dataCoverages);
	std::vector<S100::DataCoverage> GetDataCoverage();
};
