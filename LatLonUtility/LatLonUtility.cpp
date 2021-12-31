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


double LatLonUtility::GetBearingGreatCircle(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	// If the unit of the input value is degree,
	if (radian == false)
	{
		lat1 *= DEG2RAD;
		lon1 *= DEG2RAD;
		lat2 *= DEG2RAD;
		lon2 *= DEG2RAD;
	}

	double result = atan2(sin(lon2 - lon1) * cos(lat2),
		cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(lon2 - lon1));

	if (radian == false)
	{
		return result * RAD2DEG;
	}

	return result;
}

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

void LatLonUtility::CalcGreatCirclePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2)
{
	GeographicLib::Geodesic geod(
		GeographicLib::Constants::WGS84_a(),
		GeographicLib::Constants::WGS84_f());

	geod.Direct(y1, x1, azimuth, distance, *y2, *x2);
}

// return meters
double LatLonUtility::CalcGreatCircleDistanceByGeographicLib(
	double x1,
	double y1,
	double x2,
	double y2)
{
	GeographicLib::Geodesic geod(
		GeographicLib::Constants::WGS84_a(),
		GeographicLib::Constants::WGS84_f());

	double s12 = 0;
	geod.Inverse(y1, x1, y2, x2, s12);

	return s12;
}

double LatLonUtility::GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	if (true == radian)
	{
		lat1 *= RAD2DEG;
		lon1 *= RAD2DEG;
		lat2 *= RAD2DEG;
		lon2 *= RAD2DEG;
	}

	auto distance = CalcRhumbLineDistanceByGeographicLib(lon1, lat1, lon2, lat2);

	return distance / 1000.0;
}

double LatLonUtility::GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian, UNIT unitReturnValue)
{
	// km
	auto distance = GetDistanceRhumbLineMercator(lat1, lon1, lat2, lon2, radian);

	switch (unitReturnValue)
	{
	case m:
		distance *= 1000.0;
		break;
	case km:
		break;
	case nautical_mile:
		distance *= 0.539957;
		break;
	default:
		break;
	}

	return distance;
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

LatLonUtility::Pos LatLonUtility::GetDestinationPointRhumbLineMercator(double lat, double lon, double bearing, double dist, bool radian)
{
	if (radian == true)
	{
		bearing *= RAD2DEG;
		lat *= RAD2DEG;
		lon *= RAD2DEG;
	}

	auto result = GetDestinationPointRhumbLineMercatorByGeographicLib(lat, lon, bearing, dist);
	return result;
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

double LatLonUtility::DMStoDegree(double _D, double _M, double _S)
{
	double D = abs(_D);

	// 1 minute = (1/60)
	// 1 second = (1/3600)
	double ret = D + (_M / 60) + (_S / 3600);
	if (_D < 0)
	{
		ret *= -1;
	}

	return ret;
}


void LatLonUtility::DegreeToDMS(const double _deg, double& _D, double& _M, double& _S)
{
	double deg = abs(_deg);

	// Only the essence part of "Degrees" is taken.
	_D = (int)deg; // D

	// The unit of tmp is minutes.
	double tmp = (deg - _D) * 60;

	// Only the integer part of tmp is taken.
	_M = (int)tmp; // M

	// S
	_S = (tmp - _M) * 60;

	if (_deg < 0)
	{
		_D *= -1;
	}
}

void LatLonUtility::Centroid(double* _x, double* _y, int _cnt, double& _resX, double& _resY)
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	for (int i = 0; i < _cnt; i++)
	{
		j = (i + 1) % _cnt;

		x1 = _x[i];
		y1 = _y[i];
		x2 = _x[j];
		y2 = _y[j];

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	_resX = centerX;
	_resY = centerY;
}

#pragma warning(disable: 4129)
double LatLonUtility::GetDegree(const std::wstring& _str)
{
	double deg = 0;
	double min = 0;
	double sec = 0;

	std::wstring str = _str;

	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

	std::wregex rx_D(L"^\\s*[+-]?\\s*\\d+\\.?\\d*\\s*$"); // D
	std::wregex rx_DM(L"^\\s*[+-]?\\s*\\d+\\.?\\d*\\s*\\/\\s*\\d+\\.?\\d*\\s*$"); // D/M
	std::wregex rx_DMS(L"^\\s*[+-]?\\s*\\d+\\.?\\d*\\s*\\/\\s*\\d+\\.?\\d*\\s*\\/\\s*\\d+\\.?\\d*\\s*$"); // D/M/S
	std::wregex rx_D1(L"s*[+-]?\\s*\\d+\\.?\\d*\\s*[¡Æ¨¬]{1}\\s*"); // D¨¬
	std::wregex rx_DM1(L"\\s*[+-]?\\s*\\d+\\.?\\d*\\s*[¡Æ¨¬]{1}\\s*\\d+\\.?\\d*\\s*\'\\s*"); // D¨¬ M'
	std::wregex rx_DMS1(L"\\s*[+-]?\\s*\\d+\\.?\\d*\\s*[¡Æ¨¬]{1}\\s*\\d+\\.?\\d*\\s*'\\s*\\d+\\.?\\d*\\s*[\"\\¡±]{1}\\s*"); // D¨¬ M' S"
	std::wregex rx_DMS2(L"\\d+\\.?\\d*-\\d+\\.?\\d*-\\d+\\.?\\d*[WESNwesn]");
	std::wregex rx_DMS3(L"\\d+\\.?\\d*-\\d+\\.?\\d*[WESNwesn]");

	wchar_t* buff = nullptr;

	if (std::regex_match(str, rx_D))
	{
		deg = std::stod(str.c_str());
	}
	else if (std::regex_match(str, rx_DM))
	{
		buff = wcstok((wchar_t*)(&str[0]), L"\/", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"\/", 0);
		min = std::stod(buff);
	}
	else if (std::regex_match(str, rx_DMS))
	{
		buff = wcstok((wchar_t*)(&str[0]), L"\/", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"\/", 0);
		min = std::stod(buff);
		buff = wcstok(nullptr, L"\/", 0);
		sec = std::stod(buff);
	}
	else if (std::regex_match(str, rx_D1))
	{
		deg = std::stod(str.c_str());
	}
	else if (std::regex_match(str, rx_DM1))
	{
		buff = wcstok((wchar_t*)(&str[0]), L"¨¬¡Æ", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"\'", 0);
		min = std::stod(buff);
	}
	else if (std::regex_match(str, rx_DMS1))
	{
		buff = wcstok((wchar_t*)(&str[0]), L"¨¬¡Æ", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"\'", 0);
		min = std::stod(buff);
		buff = wcstok(nullptr, L"\"\\¡±", 0);
		sec = std::stod(buff);
	}
	else if (std::regex_match(str, rx_DMS2))
	{
		str.erase(std::remove(str.begin(), str.end(), ','), str.end());
		buff = wcstok(&str[0], L"-", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"-", 0);
		min = std::stod(buff);
		buff = wcstok(nullptr, L"-", 0);
		sec = std::stod(buff);

		auto foundW = _str.find(L"W");
		auto foundw = _str.find(L"w");
		auto foundS = _str.find(L"S");
		auto founds = _str.find(L"s");
		if ((foundW != std::wstring::npos) || (foundw != std::wstring::npos) || (foundS != std::wstring::npos) || (founds != std::wstring::npos))
		{
			deg *= -1;
		}
	}
	else if (std::regex_match(str, rx_DMS3))
	{
		str.erase(std::remove(str.begin(), str.end(), ','), str.end());
		buff = wcstok(&str[0], L"-", 0);
		deg = std::stod(buff);
		buff = wcstok(nullptr, L"-", 0);
		min = std::stod(buff);

		auto foundW = _str.find(L"W");
		auto foundw = _str.find(L"w");
		auto foundS = _str.find(L"S");
		auto founds = _str.find(L"s");
		if ((foundW != std::wstring::npos) || (foundw != std::wstring::npos) || (foundS != std::wstring::npos) || (founds != std::wstring::npos))
		{
			deg *= -1;
		}
	}

	return DMStoDegree(deg, min, sec);
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


bool LatLonUtility::CheckFormWGS(std::wstring _wgs, double& _latDeg, double& _latMin, double& _latSec, double& _lonDeg, double& _lonMin, double& _lonSec)
{
	std::wregex rx(L"^[\\s\\S]*[WESNwesn],[\\s\\S]*[WESNwesn]$");

	if (!std::regex_match(_wgs, rx))
	{

	}
	return true;
}


bool LatLonUtility::CheckFormWGS(std::wstring _wgs, double& _deg, double& _min, double& _sec)
{
	_wgs.erase(std::remove(_wgs.begin(), _wgs.end(), ' '), _wgs.end());

	// Function: Both D¡ÆM'S"N and D-M-SN forms are satisfied.
	// Original  : ^\d+.?\d*([¨¬\-'¡Æ¡±¡¯]\d*.?\d*){0,2}["¡±¡¯]?"?[WESNwesn]$
	std::wregex rx(L"^\\s*\\d+.?\\d*\\s*([¨¬\\-'¡Æ¡¯]\\s*\\d+\\.?\\d*\\s*){0,2}[\"¡±'¡¯]*\\s*[WESNwesn]\\s*$");

	if (!std::regex_match(_wgs, rx))
	{
		return false;
	}

	// If the shape is right, extract the value.
	wchar_t* buff = nullptr;
	std::vector<double> dms;

	buff = wcstok((wchar_t*)(&_wgs[0]), L"¨¬-'¡Æ\"¡±¡¯ ", 0);

	while (buff != nullptr)
	{
		try {
			double tmp = std::stod(buff);
			dms.push_back(tmp);
		}
		catch (std::invalid_argument) {
			dms.push_back(0);
		}
		catch (std::out_of_range) {
			dms.push_back(0);
		}

		buff = wcstok(nullptr, L"¨¬-'¡Æ\"¡±¡¯ ", 0);
	}

	try {
		_deg = dms.at(0);
	}
	catch (std::out_of_range) {
		_deg = 0;
	}

	try {
		_min = dms.at(1);
	}
	catch (std::out_of_range) {
		_min = 0;
	}

	try {
		_sec = dms.at(2);
	}
	catch (std::out_of_range) {
		_sec = 0;
	}

	auto foundW = _wgs.find(L"W");
	auto foundw = _wgs.find(L"w");
	auto foundS = _wgs.find(L"S");
	auto founds = _wgs.find(L"s");
	if ((foundW != std::wstring::npos) || (foundw != std::wstring::npos) || (foundS != std::wstring::npos) || (founds != std::wstring::npos))
	{
		_deg *= -1;
		_min *= -1;
		_sec *= -1;
	}

	return true;
}


bool LatLonUtility::CheckFormWGS(std::wstring _wgs, std::wstring& _lat, std::wstring& _lon)
{
	_wgs.erase(std::remove(_wgs.begin(), _wgs.end(), ' '), _wgs.end());
	_wgs.erase(std::remove(_wgs.begin(), _wgs.end(), ','), _wgs.end());

	std::wregex rx(L"^\\d+.?\\d*-?\\d*\\d*-?\\d*.?\\d*[SNsn]\\d+.?\\d*-?\\d*\\d*-?\\d*.?\\d*[WEwe]$");

	if (!std::regex_match(_wgs, rx))
	{
		return false;
	}

	wchar_t* lat = nullptr;
	wchar_t* lon = nullptr;

	bool bNorth = true;
	bool bEast = true;

	if ((_wgs.find(L"S") != std::wstring::npos) || (_wgs.find(L"s") != std::wstring::npos))
	{
		bNorth = false;
	}

	if ((_wgs.find(L"W") != std::wstring::npos) || (_wgs.find(L"w") != std::wstring::npos))
	{
		bEast = false;
	}

	lat = wcstok(&_wgs[0], L"SNsn", 0);
	lon = wcstok(NULL, L"WEwe", 0);

	_lat = lat;
	_lon = lon;

	if (bNorth)
	{
		_lat.append(L"N");
	}
	else
	{
		_lat.append(L"S");
	}

	if (bEast)
	{
		_lon.append(L"E");
	}
	else
	{
		_lon.append(L"W");
	}

	return true;
}


bool LatLonUtility::CheckFormWGS(std::wstring _wgs)
{
	_wgs.erase(std::remove(_wgs.begin(), _wgs.end(), ' '), _wgs.end());
	_wgs.erase(std::remove(_wgs.begin(), _wgs.end(), ','), _wgs.end());

	std::wregex rx(L"^\\d+.?\\d*-\\d+.?\\d*-\\d+.?\\d*[SNsn]\\d+.?\\d*-\\d+.?\\d*-\\d+.?\\d*[WEwe]$");

	if (!std::regex_match(_wgs, rx))
	{
		return false;
	}

	return true;
}


std::wstring LatLonUtility::LatLonToWGS(double _lat, double _lon)
{
	double _latD = 0;
	double _latM = 0;
	double _latS = 0;

	double _lonD = 0;
	double _lonM = 0;
	double _lonS = 0;

	DegreeToDMS(_lat, _latD, _latM, _latS);
	DegreeToDMS(_lon, _lonD, _lonM, _lonS);

	std::wostringstream o;
	std::wstring wgs;

	o << (int)abs(_latD) << L"-" << (int)_latM << L"-" << std::fixed << _latS;

	if (_lat >= 0)
	{
		o << L"N,";
	}
	else
	{
		o << L"S,";
	}

	o << (int)abs(_lonD) << L"-" << (int)_lonM << L"-" << std::fixed << _lonS;

	if (_lon >= 0)
	{
		o << L"E";
	}
	else
	{
		o << L"W";
	}

	wgs = o.str();
	return wgs;
}


std::wstring LatLonUtility::LatToWGS(double _lat)
{
	double d = 0;
	double m = 0;
	double s = 0;

	DegreeToDMS(_lat, d, m, s);

	std::wostringstream o;

	o << std::fixed;
	o.precision(2);
	o << (int)abs(d) << L"-" << (int)m << L"-" << s;

	if (_lat >= 0)
	{
		o << L"N";
	}
	else
	{
		o << L"S";
	}

	return o.str();
}


std::wstring LatLonUtility::LonToWGS(double _lon)
{
	double d = 0;
	double m = 0;
	double s = 0;

	DegreeToDMS(_lon, d, m, s);

	std::wostringstream o;

	o << std::fixed;
	o.precision(2);
	o << (int)abs(d) << L"-" << (int)m << L"-" << s;

	if (_lon >= 0)
	{
		o << L"E";
	}
	else
	{
		o << L"W";
	}

	return o.str();
}


void LatLonUtility::InverseProjectionToRadian(double& _x, double& _y)
{
	double pi = M_PI;
	double RADDEG = (180.0 / pi);

	_y = 2 * (atan(exp(_y)) - (pi / 4.0));
}


void LatLonUtility::InverseProjectionToDegree(double& _x, double& _y)
{
	InverseProjectionToRadian(_x, _y);
	_x *= RAD2DEG;
	_y *= RAD2DEG;
}


void LatLonUtility::ProjectionFromRadian(double& _x, double& _y)
{
	double pi = M_PI;
	double ex = (pi / 180.0);
	double limit = 85.0 * ex;

	if (_y >= limit) {
		_y = limit;
	}

	if (_y <= -limit) {
		_y = -limit;
	}

	_y = log(tan((_y / 2.0) + (pi / 4.0)));
}


void LatLonUtility::ProjectionFromDegree(double& _x, double& _y)
{
	_x *= DEG2RAD;
	_y *= DEG2RAD;

	ProjectionFromRadian(_x, _y);
}


std::wstring LatLonUtility::GetFileExtension(std::wstring _filePath)
{
	return _filePath.substr(_filePath.find_last_of(L".") + 1);
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