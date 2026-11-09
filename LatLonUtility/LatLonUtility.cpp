#include "LatLonUtility.h"
#include "Geodesy.h"
#include <cmath>
#include <cfloat>
#include <regex>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>

#include <span>
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
	// Geodesic, not rhumb: kept so callers get the positions they always got.
	const auto destination = s100::geodesy::GeodesicDirect({ lat, lon }, bearing, dist);
	return Pos(destination.lon, destination.lat);
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

std::vector<std::string> LatLonUtility::Split(std::string targetStr, std::string token)
{
	// Check parameters
	if (token.length() == 0 || targetStr.find(token) == std::string::npos)
	{
		return std::vector<std::string>({ targetStr });
	}

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

void LatLonUtility::Split(std::string_view targetStr, std::string_view token, std::vector<std::string_view>& result)
{
	//// Check parameters
	//if (token.length() == 0 || targetStr.find(token) == std::string::npos)
	//{
	//	result.push_back(targetStr);
	//	return;
	//}

	//// return var
	//std::vector<std::string> ret;

	//int findOffset = 0;
	//int splitOffset = 0;
	//while ((splitOffset = (int)targetStr.find(token, findOffset)) != std::string::npos)
	//{
	//	result.push_back(targetStr.substr(findOffset, splitOffset - findOffset));
	//	findOffset = splitOffset + (int)token.length();
	//}

	//result.push_back(targetStr.substr(findOffset, targetStr.length() - findOffset));
	result.clear();
	// 토큰이 비어있으면 원본 전체를 하나로 반환
	if (token.empty()) {
		result.emplace_back(targetStr);
		return;
	}

	size_t start = 0;
	while (start <= targetStr.size()) {
		// token 위치 검색
		auto pos = targetStr.find(token, start);
		if (pos == std::string_view::npos) {
			// 남은 부분을 마지막 조각으로
			result.emplace_back(targetStr.substr(start));
			break;
		}
		// token 앞부분을 한 조각으로
		result.emplace_back(targetStr.substr(start, pos - start));
		// 다음 검색 시작 지점
		start = pos + token.size();
	}
}

unsigned char* LatLonUtility::HexStringToWKB(std::string value)
{
	if (value.length() <= 0 || value.length() % 2 != 0)
	{
		return nullptr;
	}

	unsigned char* result = new unsigned char[value.length() / 2];

	for (int i = 0; i < (int)value.length() / 2; i++)
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

size_t LatLonUtility::countDigits(int n)
{
	std::string tmp;

	tmp = std::to_string(n);
	return tmp.size();
}

std::string LatLonUtility::replace_all(__in const std::string& message, __in const std::string& pattern, __in const std::string& replace)
{ 
	std::string result = message;    
	std::string::size_type pos = 0;     
	
	while ((pos = result.find(pattern)) != std::string::npos) 
	{ 
		result.replace(pos, pattern.size(), replace); 
	}     
	
	return result; 
}

void LatLonUtility::replace_string(std::string& str, const std::string& from, const std::string& to)
{
	std::string::size_type pos = 0;

	while ((pos = str.find(from)) != std::string::npos)
	{
		str.replace(pos, from.size(), to);
	}
}

std::string LatLonUtility::generate_uuid() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 15);
	static const char* v = "0123456789abcdef";
	std::stringstream ss;

	for (int i = 0; i < 8; i++) ss << v[dis(gen)];
	ss << "-";
	for (int i = 0; i < 4; i++) ss << v[dis(gen)];
	ss << "-";
	for (int i = 0; i < 4; i++) ss << v[dis(gen)];
	ss << "-";
	for (int i = 0; i < 4; i++) ss << v[dis(gen)];
	ss << "-";
	for (int i = 0; i < 12; i++) ss << v[dis(gen)];

	return ss.str();
}

std::string LatLonUtility::TrimRight(std::string& str)
{
	size_t endpos = str.find_last_not_of(" \t");

	if (std::string::npos != endpos)
		str = str.substr(0, endpos + 1);

	return str;
}

std::string LatLonUtility::DeleteXMLNamespace(std::string value)
{
	size_t pos = value.find(':');
	if (pos != std::string::npos) {
		value.erase(0, pos + 1);
	}

	return value;
}

std::string LatLonUtility::To_Lowercase(std::string s)
{
	std::transform(
		s.begin(), s.end(),        // 변환할 범위
		s.begin(),                 // 결과를 덮어쓸 위치
		[](char c) -> char {
			return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}
	);
	return s;
}

std::wstring LatLonUtility::To_Lowercase(std::wstring s)
{
	std::transform(
		s.begin(), s.end(),        // 변환할 범위
		s.begin(),                 // 결과를 덮어쓸 위치
		[](wchar_t wc) -> wchar_t {
			return std::towlower(wc);
		}
	);
	return s;
}

// ---- Upstreamed from the KHOA S-100 Viewer; thin wrappers over s100::geodesy ----

void LatLonUtility::CalcGreatCirclePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2)
{
	const auto destination = s100::geodesy::GeodesicDirect({ y1, x1 }, azimuth, distance);
	*x2 = destination.lon;
	*y2 = destination.lat;
}

double LatLonUtility::CalcGreatCircleDistanceByGeographicLib(double x1, double y1, double x2, double y2)
{
	return s100::geodesy::GeodesicInverse({ y1, x1 }, { y2, x2 }).distance;
}

double LatLonUtility::GetBearingGreatCircle(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	// Spherical formula, kept as it was in the viewer so existing results do not move.
	// s100::geodesy::GeodesicInverse gives the ellipsoidal initial bearing.
	if (radian == false)
	{
		lat1 *= DEG2RAD;
		lon1 *= DEG2RAD;
		lat2 *= DEG2RAD;
		lon2 *= DEG2RAD;
	}

	const double result = std::atan2(std::sin(lon2 - lon1) * std::cos(lat2),
		std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(lon2 - lon1));

	return radian ? result : result * RAD2DEG;
}

double LatLonUtility::GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian)
{
	return GetDistanceRhumbLineMercator(lat1, lon1, lat2, lon2, radian, UNIT::km);
}

double LatLonUtility::GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian, UNIT unitReturnValue)
{
	if (radian)
	{
		lat1 *= RAD2DEG;
		lon1 *= RAD2DEG;
		lat2 *= RAD2DEG;
		lon2 *= RAD2DEG;
	}

	const double metres = s100::geodesy::RhumbInverse({ lat1, lon1 }, { lat2, lon2 }).distance;
	switch (unitReturnValue)
	{
	case UNIT::m:             return metres;
	case UNIT::nautical_mile: return s100::geodesy::FromMetres(metres, s100::geodesy::LengthUnit::NauticalMile);
	case UNIT::km:
	default:                  return s100::geodesy::FromMetres(metres, s100::geodesy::LengthUnit::Kilometre);
	}
}

LatLonUtility::Pos LatLonUtility::GetDestinationPointRhumbLineMercator(double lat, double lon, double bearing, double dist, bool radian)
{
	if (radian)
	{
		bearing *= RAD2DEG;
		lat *= RAD2DEG;
		lon *= RAD2DEG;
	}
	// Historical behaviour: despite the name this follows a geodesic (see the header).
	return GetDestinationPointRhumbLineMercatorByGeographicLib(lat, lon, bearing, dist);
}

LatLonUtility::Pos LatLonUtility::GetDestinationPointGreateCircle(double lat, double lon, double bearing, double dist, bool radian)
{
	if (radian)
	{
		bearing *= RAD2DEG;
		lat *= RAD2DEG;
		lon *= RAD2DEG;
	}
	const auto destination = s100::geodesy::GeodesicDirect({ lat, lon }, bearing, dist);
	return Pos(destination.lon, destination.lat);
}

double LatLonUtility::DMStoDegree(double _D, double _M, double _S)
{
	return s100::geodesy::DmsToDegrees(_D, _M, _S);
}

void LatLonUtility::DegreeToDMS(const double _deg, double& _D, double& _M, double& _S)
{
	const auto dms = s100::geodesy::DegreesToDms(_deg);
	_D = dms.degrees;
	_M = dms.minutes;
	_S = dms.seconds;
}

void LatLonUtility::Centroid(double* _x, double* _y, int _cnt, double& resX, double& resY)
{
	if (!_x || !_y || _cnt <= 0)
	{
		return;
	}
	const auto count = static_cast<std::size_t>(_cnt);
	const auto centre = s100::geodesy::PolygonCentroid(std::span<const double>(_x, count), std::span<const double>(_y, count));
	resX = centre.lon;
	resY = centre.lat;
}
