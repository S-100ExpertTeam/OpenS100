#pragma once
#include <vector>

#include "MBR.h"

class GeoPoint;

class Scaler
{
public:
	Scaler();
	virtual ~Scaler();

public:
	static const long long COMF = 10000000000;

	// center map
	double mox = 0;
	double moy = 0;

	// map range
	double mxMin = 0;
	double myMin = 0;
	double mxMax = 0;
	double myMax = 0;

	// Information for calculating MBR when rotated.
	double mxMinCalcMBR = 0;
	double myMinCalcMBR = 0;
	double mxMaxCalcMBR = 0;
	double myMaxCalcMBR = 0;

	// center of screen
	double sox = 0;
	double soy = 0;

	// map range
	double sxMin = 0;
	double syMin = 0;
	double sxMax = 0;
	double syMax = 0;

	double currentScale = 0;

	double mxMinLimit = -179.9;
	double myMinLimit = -84.9;
	double mxMaxLimit = 179.9;
	double myMaxLimit = 84.9;

private:
	double rotateDegree = 0;

public:
	/*
	** Coordinate change.
	*/
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate = TRUE);

	/*
	******* Calculating the distance *******
	*/
	/*
	** function : Calculate the distance between two points on the spherical surface.
	** factor :
				Double longitude1 - Hardness of the first point (unit: radian)
				Double Latitude1 - The latitude of the first point (unit: radian)
				Double longitude 2 - Hardness of the second point (unit: radian)
				Double Latitude 2 - The latitude of the second point (unit: radians)
	** return value : The distance between the two points (km)
	*/
	//double GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2); // 사용 금지 (LatLonUtility사용)

	//double GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian); // 사용 금지 (LatLonUtility사용)

	/*
	** function : Calculate the distance between two points on a two-dimensional plane.
	** factor :
			int x1 - The x-coordinate of the first point (unit: integer)
			int y1 - The y-coordinate of the first point (unit: integer)
			int x2 - The x-coordinate of the second point (unit: integer)
			int y2 - The y-coordinate of the second point (unit: integer)
	** return value : Distance between two points (integer)
	*/
	double GetDistanceScreen(int x1, int y1, int x2, int y2);

	/*
	** Screen settings.
	*/
	// You must call me when the size of View changes (OnSize)
	void SetScreen(RECT *rect);
	void SetScreen(const RECT& rect);
	// The screen area is delivered in pixel units (Max-Min +1, +1 is because the screen coordinates are zero-base).
	int  GetScreenWidth();
	int  GetScreenHeight();
	CRect GetScreenRect();
	D2D1_RECT_F GetD2Rect();
	/*
	** Map's information shown on the screen.
	*/
	// Must be called when MBR of the entire map is changed (map file open)
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);
	void SetMap(RECT r);
	double GetMapWidth();
	double GetMapHeight();
	void GetMap(MBR *mbr);
	MBR GetMap();
	MBR GetMapCalcMBR();

	/*
	** Scale info
	*/
	int GetCurrentScale();
	void SetScale(int scale);
	void UpdateScale();
	
	/*
	** Rotation info
	*/
	double GetRotationDegree();
	void SetRotationDegree(double _value);

	/*
	** zoom in & zoom out
	** factor : %
			zoom in/zoom out standard coordinates(Screen coordinates.)
	** Example : If you want to zoom in 20%, insert "20" into the factor.
	*/
	void ZoomIn(double value);
	void ZoomIn(double value, int x, int y);
	void ZoomOut(double value);
	void ZoomOut(double value, int x, int y);

	/*
	** Moving the screen.
	*/
	// Use it to move as many pixels as you want (not completed)
	void MoveMap(int x, int y);

	// Use to position a desired point (mx, my) at a desired location (sy, sy)
	void MoveMap(int sx, int sy, double mx, double my);

	//void Projection(double *x, double *y);
	//void InverseProjection(double *x, double *y);

	/*
	** Screen rotation.
	*/
	void Rotate(LONG *sx, LONG *sy, double degree);
	void RotateMap(double degree);	// Maintain the range of rotateDegrees to [0, 360]
	void NorthUp();

	// Adjust the screen that's off.
	void AdjustScreenMap();

	void AdjustScreenMap_Internal();

	// Horizontal size of monitor (in km)
	double GetScreenWidthKM();
	bool PtInMap(double _x, double _y);
	void PrivateMoveMap(int sx, int sy, double mx, double my);
	D2D1::Matrix3x2F GetMatrix();
	D2D1::Matrix3x2F GetInverseMatrix();
	CRect WGS84MBRtoCRect(MBR& mbr);
	double GetMapXMin();
	double GetMapYMin();
	double GetMapXMax();
	double GetMapYMax();
	long long GetCOMF();
	void(*XYtoSP) (int nX, int nY, int &nLat, int &nLon, bool bPluse, bool bDirect) = nullptr;
	void(*SPtoXY) (int nX, int nY, int &nLat, int &nLon, bool bPluse, bool bDirect) = nullptr;
};