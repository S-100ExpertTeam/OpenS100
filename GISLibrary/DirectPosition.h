#pragma once

#include <vector>

class DirectPosition
{
public:
	DirectPosition();
	DirectPosition(double x, double y);
	DirectPosition(double x, double y, double z);

public:
	static const int Factor = 10000000;

public:
	std::vector<int> coordinates;

public:
	int GetDimension();

	double GetX();
	int GetXInteger();
	void SetX(double value);

	double GetY();
	int GetYInteger();
	void SetY(double value);

	double GetZ();
	int GetZInteger();
	void SetZ(double value);

	bool operator==(DirectPosition& item);
};