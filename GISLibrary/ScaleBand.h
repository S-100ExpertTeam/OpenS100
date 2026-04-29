#pragma once

class ScaleBand 
{
public:
	ScaleBand() = default;
	ScaleBand(int minDisplayScale, int optDisplayScale);

public:
	int optDisplayScale = 10000000;
	int minDisplayScale = INT_MAX;

	bool isOverlap(int scale)
	{
		return optDisplayScale < scale && scale < minDisplayScale ? true : false;
	}

	static bool CompareByScale(const ScaleBand& a, const ScaleBand& b) {
		if (a.minDisplayScale == b.minDisplayScale) {
			return a.optDisplayScale > b.optDisplayScale;
		}
		return a.minDisplayScale > b.minDisplayScale;
	}

public:
	int GetOptimumDisplayScale();
	void SetOptimumDisplayScale(int value);

	int GetMinimumDisplayScale();
	void SetMinimumDisplayScale(int value);
};