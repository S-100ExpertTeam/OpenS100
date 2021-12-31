#pragma once

namespace S100HDF5
{
	enum DataCodingFormat
	{
		None = 0,
		FixedStations = 1,
		RegularGird,
		UngeorectifiedGrid,
		MovingFlatform,
		IrregularGrid,
		VariableCellSize,
		TIN
	};
}