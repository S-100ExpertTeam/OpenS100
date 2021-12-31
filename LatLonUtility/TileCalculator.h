#pragma once

namespace LatLonUtility 
{
	class TileCalculator
	{
	public:
		static double TileXtoLon(int x, int z);
		static double TileYtoLat(int y, int z);
		static double LonToTileX(double lon, int z);
		static double LatToTileY(double lat, int z);
	};
}