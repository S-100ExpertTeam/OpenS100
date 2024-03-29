
#include "stdafx.h"

float ProjectionX(float lon)
{
	return lon * ((float)M_PI / 180);
}

float ProjectionY(float lat)
{
	if (lat < -LAT_LIMIT) {
		lat = -LAT_LIMIT;
	}
	else if (lat > LAT_LIMIT) {
		lat = LAT_LIMIT;
	}

	lat *= ((float)M_PI / 180);
	return  (float)log(tan((lat / 2) + ((float)M_PI / (float)4)));
}

D2D1_POINT_2F Projection(D2D1_POINT_2F point)
{
	D2D1_POINT_2F ret;
	ret.x = ProjectionX(point.x);
	ret.y = ProjectionY(point.y);
	return ret;
}

D2D1_POINT_2F ProjectionAndApplyFactor(D2D1_POINT_2F point)
{
	D2D1_POINT_2F ret;
	ret.x = ProjectionX(point.x) * FACTOR;
	ret.y = ProjectionY(point.y) * FACTOR;
	return ret;
}

float InverseProjectionX(float lon)
{
	return lon * (180 / (float)M_PI);
}

float InverseProjectionY(float lat)
{
	lat = (float)(2 * (atan(exp(lat)) - ((float)M_PI / (float)4)));
	return lat * (180 / (float)M_PI);
}

D2D1_POINT_2F InverseProjection(D2D1_POINT_2F point)
{
	D2D1_POINT_2F ret;
	ret.x = InverseProjectionX(point.x);
	ret.y = InverseProjectionY(point.y);
	return ret;
}