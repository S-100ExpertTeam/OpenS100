#include "stdafx.h"
#include "Scaler.h"
#include <utility>
#include <limits>
#include <cmath>
#include <array>
#include <algorithm>
#include "GeoCommonFuc.h"
#include "MBR.h"
#include "GeoPoint.h"
#include "ENCCommon.h"

#include "../LatLonUtility/LatLonUtility.h"
#include <math.h>

Scaler::Scaler(void)
{
	projection(mxMinLimit, myMinLimit);
	projection(mxMaxLimit, myMaxLimit);

	mxMin = mxMinLimit;
	myMin = myMinLimit;
	mxMax = mxMaxLimit;
	myMax = myMaxLimit;
}

Scaler::~Scaler(void)
{

}

#pragma warning(disable:4244)
void Scaler::DeviceToWorld(long sx, long sy, double *mx, double *my, bool /*rotate*/)
{
	double mxWidth = mxMax - mxMin;
	double myHeight = myMax - myMin;
	double sxWidth = sxMax - sxMin;
	double syHeight = syMax - syMin;

	double epsilon = std::numeric_limits<double>::epsilon();
	if (std::fabs(mxWidth) < epsilon ||
		std::fabs(myHeight) < epsilon ||
		std::fabs(sxWidth) < epsilon ||
		std::fabs(syHeight) < epsilon) {
		return;
	}

	double currentRatio = 0;
	double currentXRatio = mxWidth / sxWidth;
	double currentYRatio = myHeight / syHeight;

	if (currentXRatio > currentYRatio)
	{
		currentRatio = currentXRatio;
	}
	else 
	{
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	*mx = mox + (sx - sox) * currentRatio;
	*my = moy - (sy - soy) * currentRatio;
}

#pragma warning(disable:4244)
void Scaler::WorldToDevice(double mx, double my, long *sx, long *sy, bool /*rotate*/)
{
	double mxWidth = mxMax - mxMin;
	double myHeight = myMax - myMin;
	double sxWidth = sxMax - sxMin;
	double syHeight = syMax - syMin;

	double epsilon = std::numeric_limits<double>::epsilon();
	if (std::fabs(mxWidth) < epsilon ||
		std::fabs(myHeight) < epsilon ||
		std::fabs(sxWidth) < epsilon ||
		std::fabs(syHeight) < epsilon) {
		return;
	}

	double currentRatio = 0;
	double currentXRatio = mxWidth / sxWidth;
	double currentYRatio = myHeight / syHeight;
	
	if (currentXRatio > currentYRatio)
	{
		currentRatio = currentXRatio;
	}
	else
	{
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	*sx = (long)((mx - mox) / currentRatio + sox);
	*sy = (long)((moy - my) / currentRatio + soy);

}

#pragma warning(disable:4244)
void Scaler::WorldToDevice_F(double mx, double my, float *sx, float *sy, bool /*rotate*/)
{
	double mxWidth = mxMax - mxMin;
	double myHeight = myMax - myMin;
	double sxWidth = sxMax - sxMin;
	double syHeight = syMax - syMin;

	double epsilon = std::numeric_limits<double>::epsilon();
	if (std::fabs(mxWidth) < epsilon ||
		std::fabs(myHeight) < epsilon ||
		std::fabs(sxWidth) < epsilon ||
		std::fabs(syHeight) < epsilon) {
		return;
	}

	double currentRatio = 0;
	double currentXRatio = mxWidth / sxWidth;
	double currentYRatio = myHeight / syHeight;

	if (currentXRatio > currentYRatio)
	{
		currentRatio = currentXRatio;
	}
	else
	{
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	*sx = (float)((mx - mox) / currentRatio + sox);
	*sy = (float)((moy - my) / currentRatio + soy);
}

void Scaler::SetScreen(long left, long top, long right, long bottom)
{
	sxMin = left;
	syMin = top;
	sxMax = right;
	syMax = bottom;

	sox = (sxMin + sxMax) / 2;
	soy = (syMin + syMax) / 2;
	UpdateScale();
}

void Scaler::SetMap(double xmin, double ymin, double xmax, double ymax)
{
	mxMin = xmin;
	myMin = ymin;
	mxMax = xmax;
	myMax = ymax;

	mox = (mxMin + mxMax) / 2;
	moy = (myMin + myMax) / 2;

	UpdateScale();
}

void Scaler::SetMap(MBR mbr)
{
	SetMap(mbr.xmin, mbr.ymin, mbr.xmax, mbr.ymax);
}

void Scaler::SetMapFromScreen(long left, long top, long right, long bottom)
{
	double xmin = 0;
	double ymin = 0;
	double xmax = 0;
	double ymax = 0;

	DeviceToWorld(left, bottom, &xmin, &ymin);
	DeviceToWorld(right, top, &xmax, &ymax);

	SetMap(xmin, ymin, xmax, ymax);
}

#pragma warning(disable:4244)
int Scaler::GetCurrentScale()
{
	return currentScale;
}

void Scaler::UpdateScale()
{
	// The X-axis range of the map.
	double _mxMin = mxMin;
	double _mxMax = mxMax;

	// The actual distance of the X-axis of the map (km)
	double distance = (_mxMax - _mxMin) * EARTH_RADIUS;

	// The width of the screen.
	int width = GetScreenWidth();

	// km per pixel.
	double KMPerPixel = distance / (double)width;

	// Calculate the scale.
	currentScale = KMPerPixel * 1000 * 100 * DOTPERCM;
}

#pragma warning(disable:4244)
void Scaler::SetScale(int scale)
{
	// The center of the map.
	double oldMox = mox;
	double oldMoy = moy;

	// Change the map center point to latitude 0, 0. 
	MoveMap(sox, soy, 0, 0);

	// Find the actual coordinates of the center point of the screen (radian)
	inverseProjection(mox, moy);

	double finalWidthKM = GetScreenWidthKM() * (double)scale;
	double offsetXtheta = finalWidthKM / (2 * EARTH_RADIUS);

	// X coordinates are projection coordinates and radian coordinates.
	mxMin = mox - offsetXtheta;
	mxMax = mox + offsetXtheta;

	double finalHeightKM = (finalWidthKM * (double)GetScreenHeight()) / (double)GetScreenWidth();
	double offsetYtheta = finalHeightKM / (2 * EARTH_RADIUS);

	myMin = moy - offsetYtheta;
	myMax = moy + offsetYtheta;

	double yLimit = 85;

	yLimit *= DEG2RAD;

	if (myMin < -yLimit)
	{
		myMin = -yLimit;
	}

	if (myMax > yLimit)
	{
		myMax = yLimit;
	}

	myMin = log(tan((myMin / 2.0) + M_PI_D4));
	myMax = log(tan((myMax / 2.0) + M_PI_D4));
	moy = log(tan((moy / 2.0) + M_PI_D4));

	MoveMap(sox, soy, oldMox, oldMoy);

	UpdateScale();
}

std::wstring Scaler::GetFormattedScale() const
{
	// Thousands grouping of the integral scale, e.g. 1 : 12,345
	const auto digits = std::to_wstring(static_cast<long long>(currentScale));
	std::wstring grouped;
	int count = 0;
	for (auto it = digits.rbegin(); it != digits.rend(); ++it)
	{
		if (count != 0 && count % 3 == 0)
		{
			grouped.insert(grouped.begin(), L',');
		}
		grouped.insert(grouped.begin(), *it);
		++count;
	}
	return L"1 : " + grouped;
}

int Scaler::GetRotationDegree()
{
	return rotateDegree;
}

void Scaler::SetRotationDegree(int _value)
{
	rotateDegree = _value;
}

void Scaler::Rotate(int value)
{
	rotateDegree += value;
}

void Scaler::ZoomIn(double value)
{
	double old_mox = mox;
	double old_moy = moy;
	double old_mxMin = mxMin;
	double old_myMin = myMin;
	double old_mxMax = mxMax;
	double old_myMax = myMax;

	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;

	double zoomValue = (100.0 - value) / 100.0;

	mWidth *= zoomValue;
	mHeight *= zoomValue;

	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	UpdateScale();

	if (GetCurrentScale() < 500)
	{
		mox = old_mox;
		moy = old_moy;
		mxMin = old_mxMin;
		myMin = old_myMin;
		mxMax = old_mxMax;
		myMax = old_myMax;

		UpdateScale();
	}
}


void Scaler::ZoomIn(double value, int x, int y)
{
	double mx = 0;
	double my = 0;

	DeviceToWorld(x, y, &mx, &my);

	if (mx >= 0)
	{
		while (mx > (acos(-1.0)))
		{
			mx -= (2 * acos(-1.0));
		}
	}
	else
	{
		while (mx < (-acos(-1.0)))
		{
			mx += (2 * acos(-1.0));
		}
	}

	ZoomIn(value);

	MoveMap(x, y, mx, my);

	UpdateScale();
}


void Scaler::ZoomOut(double value)
{
	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;
	double zoomValue = (100.0 + value) / 100.0;

	mWidth *= zoomValue;
	mHeight *= zoomValue;

	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	AdjustScreenMap();

	UpdateScale();
}

void Scaler::ZoomOut(double value, int x, int y)
{
	double oldMx = 0;
	double oldMy = 0;

	DeviceToWorld(x, y, &oldMx, &oldMy);

	ZoomOut(value);

	MoveMap(x, y, oldMx, oldMy);

	UpdateScale();
}

#pragma warning(disable:4244)
void Scaler::MoveMap(int x, int y)
{
	MoveMap(sox + x, soy + y, mox, moy);
}

#pragma warning(disable:4244)
void Scaler::MoveMap(int sx, int sy, double mx, double my)
{
	PrivateMoveMap(sx, sy, mx, my);

	long sx1 = 0, sy1 = 0;     // On the right side of the map.
	long sx2 = 0, sy2 = 0;	   // The left side of the map.

	double x = 180;
	double y = 0;

	projection(x, y);

	WorldToDevice(x, 0, &sx1, &sy1);
	WorldToDevice(-x, 0, &sx2, &sy2);

	double xlimit = acos(-1.0);
	
	/*
	** If the scale is larger than 35 million, do not use infinite screen rotation.
	*/
	if (currentScale < 3500000)
	{
		// If the map goes to the left of the screen,
		if (mxMin > 180) 
		{
			MoveMap(sx1 - sx2, 0);
		}
		if (mxMax < -180) 
		{
			MoveMap(sx2 - sx1, 0);
		}

		return;
	}

	if (sx1 <= 0) 
	{
		PrivateMoveMap(0, sy, -xlimit, my);
	}
	// If the map goes to the right of the screen,
	else if (sx2 >= sxMax)
	{
		PrivateMoveMap(sxMax, sy, xlimit, my);
	}
}

void Scaler::PrivateMoveMap(int sx, int sy, double mx, double my)
{
	double mx1 = 0;
	double my1 = 0;

	// Map coordinates of the point you want to move.
	DeviceToWorld(sx, sy, &mx1, &my1);

	// New map center point.
	mox += (mx - mx1);
	moy += (my - my1);

	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;

	// new range of maps.
	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	// Adjustment when the y-axis is out of range.
	if (myMin < myMinLimit)		
	{
		myMin = myMinLimit;
		myMax = myMin + mHeight;
		moy = (myMin + myMax) / 2;
	}

	if (myMax > myMaxLimit)
	{
		myMax = myMaxLimit;
		myMin = myMax - mHeight;
		moy = (myMin + myMax) / 2;
	}
}

void Scaler::AdjustScreenMap()
{
	AdjustScreenMap_Internal();
}

#pragma warning(disable:4244)
void Scaler::AdjustScreenMap_Internal()
{
	DeviceToWorld(sxMin, syMax, &mxMin, &myMin);
	DeviceToWorld(sxMax, syMin, &mxMax, &myMax);
	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;

	// If the size of View or Map is not specified,
	if (((myMaxLimit - myMinLimit) == 0) || ((syMax - syMin) == 0))
	{
		return;
	}

	double mRatio = (mxMaxLimit - mxMinLimit) / (myMaxLimit - myMinLimit); // The aspect ratio of the map.
	double sRatio = (sxMax - sxMin) / (syMax - syMin); // The aspect ratio of the screen.

	// The screen is flatter.
	// width the map is full.
	if (mRatio < sRatio)
	{
		if ((mxMin > mxMinLimit) || (mxMax < mxMaxLimit))
		{
			if (((mxMax - mxMin) < (mxMaxLimit - mxMinLimit)))
			{
				return;
			}
		}

		myMax = (mxMaxLimit * (syMax - syMin)) / (sxMax - sxMin);
		myMin = (mxMinLimit * (syMax - syMin)) / (sxMax - sxMin);

		mxMin = mxMinLimit;
		mxMax = mxMaxLimit;

		mox = (mxMin + mxMax) / 2;
		moy = (myMin + myMax) / 2;
	}
	else if (mRatio == sRatio)
	{
		mxMin = mxMinLimit;
		myMin = myMinLimit;
		mxMax = mxMaxLimit;
		myMax = myMaxLimit;
	}
	// The screen is flatter.
	// height the map is full.
	else
	{
		if ((myMin > myMinLimit) || (myMax < myMaxLimit))
		{
			if ((myMax - myMin) < (myMaxLimit - myMinLimit))
			{
				return;
			}
		}

		mxMin = (myMinLimit * (sxMax - sxMin)) / (syMax - syMin);
		mxMax = (myMaxLimit * (sxMax - sxMin)) / (syMax - syMin);

		myMin = myMinLimit;
		myMax = myMaxLimit;

		mox = (mxMin + mxMax) / 2;
		moy = (myMin + myMin) / 2;
	}
}

double Scaler::GetMapWidth()
{
	return mxMax - mxMin;
}

double Scaler::GetMapHeight()
{
	return myMax - myMin;
}

void Scaler::GetMap(MBR *mbr)
{
	MBR _mbr(mxMin, myMin, mxMax, myMax);

	if (mbr == nullptr)
	{
		mbr = new MBR();
	}

	mbr->SetMBR(_mbr);
}

MBR Scaler::GetMap()
{
	if (rotateDegree == 0) {
		return MBR (mxMin, myMin, mxMax, myMax);
	}
	else {
		// Rotate the four view corners about the map origin and take their bounding box.
		const double radian = rotateDegree * DEG2RAD;
		const double sinValue = std::sin(radian);
		const double cosValue = std::cos(radian);

		const std::array<std::pair<double, double>, 4> corners{ {
			{ mxMin - mox, myMax - moy },
			{ mxMin - mox, myMin - moy },
			{ mxMax - mox, myMax - moy },
			{ mxMax - mox, myMin - moy } } };

		double mxMinCalcMBR = std::numeric_limits<double>::max();
		double mxMaxCalcMBR = std::numeric_limits<double>::lowest();
		double myMinCalcMBR = std::numeric_limits<double>::max();
		double myMaxCalcMBR = std::numeric_limits<double>::lowest();

		for (const auto& [x, y] : corners)
		{
			const double rx = y * sinValue + x * cosValue;
			const double ry = y * cosValue - x * sinValue;
			mxMinCalcMBR = std::min(mxMinCalcMBR, rx);
			mxMaxCalcMBR = std::max(mxMaxCalcMBR, rx);
			myMinCalcMBR = std::min(myMinCalcMBR, ry);
			myMaxCalcMBR = std::max(myMaxCalcMBR, ry);
		}

		return MBR(mxMinCalcMBR + mox, myMinCalcMBR + moy, mxMaxCalcMBR + mox, myMaxCalcMBR + moy);
	}
}

double Scaler::GetScreenWidthKM()
{
	int width = GetScreenWidth();
	double widthCM = (double)width / (double)DOTPERCM;

	return (double)widthCM * pow(10.0, -5);
}

#pragma warning(disable:4244)
int Scaler::GetScreenWidth() const
{
	return sxMax - sxMin + 1;
}

int Scaler::GetScreenHeight() const
{
	return syMax - syMin + 1;
}

ScreenRect Scaler::GetScreenRect() const
{
	return ScreenRect{ 0, 0, GetScreenWidth(), GetScreenHeight() };
}





bool Scaler::PtInMap(double _x, double _y)
{
	if ((_x >= mxMin) && (_x <= mxMax) && (_y >= myMin) && (_y <= myMax)) 
	{
		return true;
	}

	return false;
}

double Scaler::GetMapXMin()
{
	return mxMin;
}

double Scaler::GetMapYMin()
{
	return myMin;
}

double Scaler::GetMapXMax()
{
	return mxMax;
}

double Scaler::GetMapYMax()
{
	return myMax;
}