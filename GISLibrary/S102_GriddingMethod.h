#pragma once
enum class S102_GriddingMethod
{
	none = 0,
	basicWeightedMean = 1,
	shoalestDepth = 2,
	tpuWeightedMean = 3,
	cube = 4,
	nearestNeighbour = 5, 
	naturalNeighbour = 6, 
	polynomialTendency = 7,
	spline = 8,
	kriging
};

