/**
 * @file LatLonUtility.h
 * @brief Latitude/Longitude utility functions and geographic calculations
 * @details Provides utilities for rhumb line calculations, coordinate transformations,
 *          string manipulation, and geographic computations using GeographicLib.
 */

#ifndef LATLONUTILITY_H
#define LATLONUTILITY_H

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for C++
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cwctype>

/**
 * @namespace LatLonUtility
 * @brief Namespace containing geographic and utility functions for latitude/longitude operations
 */
namespace LatLonUtility {

#define RAD2DEG 180 / M_PI  ///< Conversion factor from radians to degrees
#define DEG2RAD M_PI / 180  ///< Conversion factor from degrees to radians
#define RADIAN 6378.1       ///< Earth radius in kilometers

	/**
	 * @struct Pos
	 * @brief Represents a geographic position with longitude and latitude
	 */
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
		double lon = 0;  ///< Longitude in degrees
		double lat = 0;  ///< Latitude in degrees
	};

	/**
	 * @enum UNIT
	 * @brief Distance unit enumeration
	 */
	enum class UNIT {
		m = 0,          ///< Meters
		km,             ///< Kilometers
		nautical_mile   ///< Nautical miles
	};

	/**
	 * @brief Calculate rhumb line distance between two geographic points using GeographicLib
	 * @param x1 Longitude of the first point in degrees
	 * @param y1 Latitude of the first point in degrees
	 * @param x2 Longitude of the second point in degrees
	 * @param y2 Latitude of the second point in degrees
	 * @return Distance in meters
	 */
	double CalcRhumbLineDistanceByGeographicLib(double x1, double y1, double x2, double y2);

	/**
	 * @brief Calculate destination point along a rhumb line using GeographicLib
	 * @param x1 Longitude of the starting point in degrees
	 * @param y1 Latitude of the starting point in degrees
	 * @param azimuth Bearing angle in degrees
	 * @param distance Distance to travel in meters
	 * @param[out] x2 Pointer to store the destination longitude in degrees
	 * @param[out] y2 Pointer to store the destination latitude in degrees
	 */
	void CalcRhumbLinePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2);
	
	/**
	 * @brief Calculate bearing along a rhumb line using Mercator projection
	 * @param lat1 Latitude of the first point
	 * @param lon1 Longitude of the first point
	 * @param lat2 Latitude of the second point
	 * @param lon2 Longitude of the second point
	 * @param radian If true, input/output in radians; if false, in degrees
	 * @return Bearing angle in the specified unit (radians or degrees)
	 */
	double GetBearingRhubmLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian);
	
	/**
	 * @brief Calculate bearing along a rhumb line using GeographicLib
	 * @param lat1 Latitude of the first point
	 * @param lon1 Longitude of the first point
	 * @param lat2 Latitude of the second point
	 * @param lon2 Longitude of the second point
	 * @param radian If true, input/output in radians; if false, in degrees
	 * @return Bearing angle in the specified unit (radians or degrees)
	 */
	double GetBearingRhubmLineMercatorByGeographicLib(double lat1, double lon1, double lat2, double lon2, bool radian);

	/**
	 * @brief Extract bearing angle between two geographic points
	 * @param lat1 Latitude of the first point in degrees
	 * @param lon1 Longitude of the first point in degrees
	 * @param lat2 Latitude of the second point in degrees
	 * @param lon2 Longitude of the second point in degrees
	 * @return Bearing angle in degrees
	 */
	double GetAngle(double lat1, double lon1, double lat2, double lon2);

	/**
	 * @brief Get destination point along a rhumb line using GeographicLib
	 * @param lat Starting latitude in degrees
	 * @param lon Starting longitude in degrees
	 * @param bearing Bearing angle in degrees
	 * @param dist Distance to travel in meters
	 * @return Destination position as a Pos struct
	 */
	Pos GetDestinationPointRhumbLineMercatorByGeographicLib(double lat, double lon, double bearing, double dist);

	/**
	 * @brief Rotate a point around a reference point
	 * @param _x X-coordinate of the point to rotate
	 * @param _y Y-coordinate of the point to rotate
	 * @param _cx X-coordinate of the rotation center
	 * @param _cy Y-coordinate of the rotation center
	 * @param[out] _mx Rotated X-coordinate
	 * @param[out] _my Rotated Y-coordinate
	 * @param _degree Rotation angle
	 * @param radian If true, angle is in radians; if false, in degrees (default: false)
	 */
	void RotatePoint(int _x, int _y, int _cx, int _cy, int& _mx, int& _my, double _degree, bool radian = false);

	/**
	 * @brief Split a string by a delimiter token
	 * @param targetStr String to split
	 * @param token Delimiter token
	 * @return Vector of split string segments
	 */
	std::vector<std::string> Split(std::string targetStr, std::string token);
	
	/**
	 * @brief Split a string_view by a delimiter token (optimized version)
	 * @param targetStr String view to split
	 * @param token Delimiter token
	 * @param[out] result Vector to store the split string_view segments
	 */
	void Split(std::string_view targetStr, std::string_view token, std::vector<std::string_view>& result);

	/**
	 * @brief Convert hexadecimal string to Well-Known Binary (WKB) format
	 * @param value Hexadecimal string (e.g., "0101000000000000000000F03F000000000000F03F")
	 * @return Pointer to unsigned char array containing WKB data (caller must delete[])
	 * @note Returns nullptr if input is invalid (empty or odd length)
	 */
	unsigned char* HexStringToWKB(std::string value);

	/**
	 * @brief Count the number of digits in an integer
	 * @param n Integer value
	 * @return Number of digits
	 */
	size_t countDigits(int n);

	/**
	 * @brief Replace all occurrences of a pattern in a string
	 * @param message Source string
	 * @param pattern Pattern to find
	 * @param replace Replacement string
	 * @return New string with all replacements applied
	 */
	std::string replace_all(__in const std::string& message, __in const std::string& pattern, __in const std::string& replace);
	
	/**
	 * @brief Replace all occurrences of a substring in-place
	 * @param[in,out] str String to modify
	 * @param from Substring to find
	 * @param to Replacement substring
	 */
	void replace_string(std::string& str, const std::string& from, const std::string& to);

	/**
	 * @brief Generate a UUID v4 string
	 * @return UUID string in format "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
	 */
	std::string generate_uuid();

	/**
	 * @brief Trim whitespace from the right side of a string
	 * @param[in,out] str String to trim
	 * @return Trimmed string
	 */
	std::string TrimRight(std::string& str);

	/**
	 * @brief Remove XML namespace prefix from a string
	 * @param value String potentially containing namespace prefix (e.g., "ns:element")
	 * @return String with namespace prefix removed (e.g., "element")
	 */
	std::string DeleteXMLNamespace(std::string value);

	std::string To_Lowercase(std::string s);
	std::wstring To_Lowercase(std::wstring s);

	// ---- Upstreamed from the KHOA S-100 Viewer. New code should use s100::geodesy (Geodesy.h). ----

	/**
	 * @brief Destination along a geodesic (great circle on the ellipsoid)
	 * @param x1 Start longitude in degrees
	 * @param y1 Start latitude in degrees
	 * @param azimuth Initial bearing in degrees
	 * @param distance Distance in metres
	 * @param[out] x2 Destination longitude in degrees
	 * @param[out] y2 Destination latitude in degrees
	 */
	void CalcGreatCirclePosByGeographicLib(double x1, double y1, double azimuth, double distance, double* x2, double* y2);

	/**
	 * @brief Geodesic distance between two points
	 * @return Distance in metres
	 */
	double CalcGreatCircleDistanceByGeographicLib(double x1, double y1, double x2, double y2);

	/**
	 * @brief Initial great-circle bearing on a sphere
	 * @param radian If true, input and output are radians; otherwise degrees
	 * @return Bearing in (-180, 180] degrees (or the radian equivalent)
	 */
	double GetBearingGreatCircle(double lat1, double lon1, double lat2, double lon2, bool radian);

	/**
	 * @brief Rhumb line distance in kilometres
	 * @param radian If true, the inputs are radians; otherwise degrees
	 */
	double GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian);

	/** @brief Rhumb line distance in the requested unit */
	double GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian, UNIT unitReturnValue);

	/**
	 * @brief Destination point at a bearing and distance
	 * @param dist Distance in metres
	 * @param radian If true, lat/lon/bearing are radians; the result is always in degrees
	 * @warning Despite the name this follows a GEODESIC, like
	 *          GetDestinationPointRhumbLineMercatorByGeographicLib which it calls. Both the KHOA viewer
	 *          and OpenS100 have always behaved that way, so it is kept until the callers (route legs,
	 *          AIS vectors, augmented rays) are reviewed. For a real rhumb line use
	 *          s100::geodesy::RhumbDirect or CalcRhumbLinePosByGeographicLib.
	 */
	Pos GetDestinationPointRhumbLineMercator(double lat, double lon, double bearing, double dist, bool radian);

	/**
	 * @brief Destination along a geodesic
	 * @param dist Distance in metres
	 * @param radian If true, lat/lon/bearing are radians; the result is always in degrees
	 */
	Pos GetDestinationPointGreateCircle(double lat, double lon, double bearing, double dist, bool radian);

	/** @brief Degrees/minutes/seconds to decimal degrees; the sign of `_D` applies to the whole value */
	double DMStoDegree(double _D, double _M, double _S);

	/** @brief Decimal degrees to degrees/minutes/seconds; `_D` carries the sign */
	void DegreeToDMS(const double _deg, double& _D, double& _M, double& _S);

	/** @brief Area-weighted centroid of a polygon given as coordinate arrays */
	void Centroid(double* _x, double* _y, int _cnt, double& resX, double& resY);
};
#endif 