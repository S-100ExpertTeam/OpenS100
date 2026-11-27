#pragma once
#include <string>


#include "ScreenTypes.h"
#include <vector>

#include "MBR.h"

class GeoPoint;

class Scaler
{
public:
	Scaler();
	virtual ~Scaler();

public:

	// center map
	double mox = 0;
	double moy = 0;

	// map range
	double mxMin = 0;
	double myMin = 0;
	double mxMax = 0;
	double myMax = 0;

	// center of screen
	double sox = 0;
	double soy = 0;

	// map range
	double sxMin = 0;
	double syMin = 0;
	double sxMax = 0;
	double syMax = 0;

	double currentScale = 0;

	// The world the chart projection can show (ProjectionLimits).
	double mxMinLimit = 0;
	double myMinLimit = 0;
	double mxMaxLimit = 0;
	double myMaxLimit = 0;
	// Takes the limits of the chart projection again: after it changed.
	void UpdateLimits();

private:
	int rotateDegree = 0;
	double rotateSin = 0;
	double rotateCos = 1;

	bool PixelSize(double& ratio);
	bool FromCentre(double mx, double my, double& dx, double& dy, bool rotate);

public:
	/*
	** Coordinate change. The view may be turned (SetRotationDegree): a point of the map goes
	** to where it is on the turned screen, and a point of the screen to where it is on the map.
	** With rotate false the turn is left out (the screen as it would be with north up).
	*/
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = true);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = true);
	void WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate = true);

	/*
	** Screen settings.
	*/
	// You must call me when the size of View changes (OnSize)
	void SetScreen(long left, long top, long right, long bottom);
	// The screen area is delivered in pixel units (Max-Min +1, +1 is because the screen coordinates are zero-base).
	int  GetScreenWidth() const;
	int  GetScreenHeight() const;
	ScreenRect GetScreenRect() const;
	
	/*
	** Map's information shown on the screen.
	*/
	// Must be called when MBR of the entire map is changed (map file open)
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);
	// Map extent from a screen rectangle (device coordinates).
	void SetMapFromScreen(long left, long top, long right, long bottom);
	double GetMapWidth();
	double GetMapHeight();
	void GetMap(MBR *mbr);
	MBR GetMap();
	
	/*
	** Scale info
	*/
	int GetCurrentScale();
	void SetScale(int scale);
	void UpdateScale();
	// "1 : 12,345"
	std::wstring GetFormattedScale() const;
	
	/*
	** Rotation info
	*/
	// The picture is turned clockwise about the centre of the screen by this many degrees
	// (0 to 359): north is that far clockwise from up.
	int GetRotationDegree();
	void SetRotationDegree(int _value);
	void Rotate(int value);
	// Where north is on the screen at a point of the map, in degrees clockwise from up: the
	// turn of the view and, about a pole, of the projection (NorthAngle).
	double NorthOnScreen(double mx, double my) const;

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

	// Adjust the screen that's off.
	void AdjustScreenMap();

	void AdjustScreenMap_Internal();

	// Horizontal size of monitor (in km)
	double GetScreenWidthKM();
	bool PtInMap(double _x, double _y);
	void PrivateMoveMap(int sx, int sy, double mx, double my);
	double GetMapXMin();
	double GetMapYMin();
	double GetMapXMax();
	double GetMapYMax();
};