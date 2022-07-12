#include "LatLonUtility.h"
#include <cmath>
#include <cfloat>
#include <regex>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>

#include <GeographicLib/PolygonArea.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Constants.hpp>

double LatLonUtility::CalcRhumbLineDistanceByGeographicLib(double x1, double y1, double x2, double y2)
{
	GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

	double s12 = 0;
	double azi12 = 0;

	rhumb.Inverse(y1, x1, y2, x2, s12, azi12);

	return s12;
}

void LatLonUtility::CalcRhumbLinePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2)
{
	GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

	rhumb.Direct(y1, x1, azimuth, distance, *y2, *x2);
}

double LatLonUtility::GetBearingRhubmLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	return GetBearingRhubmLineMercatorByGeographicLib(lat1, lon1, lat2, lon2, radian);

	// If the unit of the input value is degree,
	if (radian == false)
	{
		lat1 *= DEG2RAD;
		lon1 *= DEG2RAD;
		lat2 *= DEG2RAD;
		lon2 *= DEG2RAD;
	}

	double dLon = lon2 - lon1;

	// if dLon over M_PI take shorter rhumb line across the anti-meridian
	if (abs(dLon) > M_PI)
	{
		dLon = (dLon > 0) ? -(2 * M_PI - dLon) : (2 * M_PI + dLon);
	}

	double d = log(tan((lat2 / 2) + (M_PI / 4)) / tan((lat1 / 2) + (M_PI / 4)));

	double result = atan2(dLon, d);

	if (radian == false)
	{
		return result * RAD2DEG;
	}

	return result;
}

double LatLonUtility::GetBearingRhubmLineMercatorByGeographicLib(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	// If the unit of the input value is radian,
	if (radian == true)
	{
		lat1 *= RAD2DEG;
		lon1 *= RAD2DEG;
		lat2 *= RAD2DEG;
		lon2 *= RAD2DEG;
	}

	GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

	double s12 = 0;
	double azi12 = 0;

	rhumb.Inverse(lat1, lon1, lat2, lon2, s12, azi12);

	return azi12;
}

double LatLonUtility::GetAngle(double lat1, double lon1, double lat2, double lon2)
{
	return GetBearingRhubmLineMercator(lat1, lon1, lat2, lon2, false);
}

LatLonUtility::Pos LatLonUtility::GetDestinationPointRhumbLineMercatorByGeographicLib(
	double lat, double lon, double bearing, double dist)
{
	LatLonUtility::Pos result;

	GeographicLib::Geodesic geod(
		GeographicLib::Constants::WGS84_a(),
		GeographicLib::Constants::WGS84_f());

	double s12 = 0;
	double azi12 = 0;

	geod.Direct(lat, lon, bearing, dist, result.lat, result.lon);

	return result;
}

void LatLonUtility::RotatePoint(int _x, int _y, int _cx, int _cy, int& _mx, int& _my, double _degree, bool radian)
{
	if (!radian)
	{
		_degree *= DEG2RAD;
	}

	_mx = (int)(((_x - _cx) * cos(_degree) - (_y - _cy) * sin(_degree)) + _cx);
	_my = (int)(((_x - _cx) * sin(_degree) + (_y - _cy) * cos(_degree)) + _cy);
}

//wchar_t to char.
char* LatLonUtility::ConvertWCtoC(wchar_t* str)
{
	char* pStr = nullptr;

	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	if (strSize <= 0)
	{
		return nullptr;
	}

	pStr = new char[strSize];

	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}


///////////////////////////////////////////////////////////////////////
//char to wchar_t
wchar_t* LatLonUtility::ConvertCtoWC(char* str)
{
	wchar_t* pStr = nullptr;
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	if (strSize <= 0)
	{
		return nullptr;
	}

	pStr = new WCHAR[strSize];
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}

std::vector<std::string> LatLonUtility::Split(std::string targetStr, std::string token)
{
	// Check parameters
	if (token.length() == 0 || targetStr.find(token) == std::string::npos)
		return std::vector<std::string>({ targetStr });

	// return var
	std::vector<std::string> ret;

	int findOffset = 0;
	int splitOffset = 0;
	while ((splitOffset = (int)targetStr.find(token, findOffset)) != std::string::npos)
	{
		ret.push_back(targetStr.substr(findOffset, splitOffset - findOffset));
		findOffset = splitOffset + (int)token.length();
	}
	ret.push_back(targetStr.substr(findOffset, targetStr.length() - findOffset));

	return ret;
}

char* LatLonUtility::HexStringToWKB(std::string value)
{
	if (value.length() <= 0 || value.length() % 2 != 0)
	{
		return nullptr;
	}

	char* result = new char[value.length() / 2];

	for (int i = 0; i < value.length() / 2; i++)
	{
		char first = value[i * 2];
		char second = value[i * 2 + 1];

		if (first < 58)
		{
			first -= 48;
		}
		else if (first > 64 && first <= 70)
		{
			first -= 55;
		}
		else if (first > 96)
		{
			first -= 87;
		}

		if (second < 58)
		{
			second -= 48;
		}
		else if (second > 64 && second <= 70)
		{
			second -= 55;
		}
		else if (second > 96)
		{
			second -= 87;
		}

		result[i] = (first << 4) | second;
	}

	return result;
}