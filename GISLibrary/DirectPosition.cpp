#include "stdafx.h"
#include "DirectPosition.h"

const double Factor = 10000000;

DirectPosition::DirectPosition()
{
	coordinates.push_back(0);
	coordinates.push_back(0);
	coordinates.push_back(0);
}

DirectPosition::DirectPosition(const DirectPosition& item)
{
	this->coordinates = item.coordinates;
}

DirectPosition::DirectPosition(double x, double y)
{
	coordinates.push_back((int)(x * Factor));
	coordinates.push_back((int)(y * Factor));
}

DirectPosition::DirectPosition(double x, double y, double z)
{
	coordinates.push_back((int)(x * Factor));
	coordinates.push_back((int)(y * Factor));
	coordinates.push_back((int)(z * Factor));
}

DirectPosition::~DirectPosition()
{

}

int DirectPosition::GetDimension()
{
	return (int)coordinates.size();
}

double DirectPosition::GetX()
{
	return GetXInteger() / Factor;
}

int DirectPosition::GetXInteger()
{
	return coordinates.at(0);
}

void DirectPosition::SetX(double value)
{
	coordinates[0] = (int)(value * Factor);
}

double DirectPosition::GetY()
{
	return GetYInteger() / Factor;
}

int DirectPosition::GetYInteger()
{
	return coordinates.at(1);
}

void DirectPosition::SetY(double value)
{
	coordinates[1] = (int)(value * Factor);
}

double DirectPosition::GetZ()
{
	return GetZInteger() / Factor;
}

int DirectPosition::GetZInteger()
{
	return coordinates.at(2);
}

void DirectPosition::SetZ(double value)
{
	if (GetDimension() == 2)
	{
		coordinates.push_back((int)(value * Factor));
	}
	
	coordinates[2] = (int)(value * Factor);
}

bool DirectPosition::operator==(DirectPosition& item)
{
	if (coordinates.size() == item.coordinates.size())
	{
		if (GetXInteger() == item.GetXInteger() &&
			GetYInteger() == item.GetYInteger())
		{
			if (coordinates.size() == 3)
			{
				if (GetZInteger() != item.GetZInteger())
				{
					return false;
				}
			}
			
			return true;
		}
	}

	return false;
}