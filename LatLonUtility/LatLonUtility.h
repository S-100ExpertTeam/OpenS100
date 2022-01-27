#ifndef LATLONUTILITY_H
#define LATLONUTILITY_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cmath>
#include <math.h>
#include <vector>

namespace LatLonUtility {

#define RAD2DEG 180 / M_PI
#define DEG2RAD M_PI / 180
#define RADIAN 6378.1


	struct Pos {
		Pos() {
			lon = 0;
			lat = 0;
		}
		Pos(double _lon, double _lat) {
			lon = _lon;
			lat = _lat;
		}
		virtual ~Pos() {}
		double lon = 0;
		double lat = 0;
	};

	enum UNIT {
		m = 0,
		km,
		nautical_mile
	};

	// return meters
	// input : degree
	double CalcRhumbLineDistanceByGeographicLib(double x1, double y1, double x2, double y2);

	// azimuth : degree
	// distance : meters
	void CalcRhumbLinePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2);
	
	// distance : km
	double GetBearingRhubmLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian);
	double GetBearingRhubmLineMercatorByGeographicLib(double lat1, double lon1, double lat2, double lon2, bool radian);

	//extracts angles.
	double GetAngle(double lat1, double lon1, double lat2, double lon2);

	Pos GetDestinationPointRhumbLineMercatorByGeographicLib(double lat, double lon, double bearing, double dist);

	// Function : A specific point (_x, _y) is rotated by _degrees around the reference point (_cx, _cy) by a reference point (_cx, _cy).
	// factor : If it's radio, then _degrees are radio units, if it's false, then degree units
	void RotatePoint(int _x, int _y, int _cx, int _cy, int& _mx, int& _my, double _degree, bool radian = false);

	//wchar_t to char
	char * ConvertWCtoC(wchar_t* str);
	//char to wchar_t
	wchar_t* ConvertCtoWC(char* str);

	std::vector<std::string> Split(std::string targetStr, std::string token);
};
#endif 