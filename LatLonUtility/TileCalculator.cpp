#include "TileCalculator.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace LatLonUtility 
{
	double TileCalculator::TileXtoLon(int x, int z)
	{
		return x / (double)(1 << z) * 360.0 - 180;
	}

	double TileCalculator::TileYtoLat(int y, int z)
	{
		double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
		return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
	}

	double TileCalculator::LonToTileX(double lon, int z)
	{
		return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
	}

	double TileCalculator::LatToTileY(double lat, int z)
	{
		double latrad = lat * M_PI / 180.0;
		return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z)));
	}
}