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
	UpdateLimits();

	mxMin = mxMinLimit;
	myMin = myMinLimit;
	mxMax = mxMaxLimit;
	myMax = myMaxLimit;
}

Scaler::~Scaler(void)
{

}

void Scaler::UpdateLimits()
{
	const MBR limits = ProjectionLimits();
	mxMinLimit = limits.xmin;
	myMinLimit = limits.ymin;
	mxMaxLimit = limits.xmax;
	myMaxLimit = limits.ymax;
}

// The size of a pixel in the units of the map: the map range fits the screen in both directions,
// so the larger of the two ratios wins. False when the map or the screen has no size. The
// centres of the map and of the screen are taken from their ranges on the way.
bool Scaler::PixelSize(double& ratio)
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
		return false;
	}

	double currentXRatio = mxWidth / sxWidth;
	double currentYRatio = myHeight / syHeight;
	ratio = currentXRatio > currentYRatio ? currentXRatio : currentYRatio;

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;
	return true;
}

// Where a point of the map is from the centre of the screen, in pixels.
bool Scaler::FromCentre(double mx, double my, double& dx, double& dy, bool rotate)
{
	double ratio = 0;
	if (!PixelSize(ratio))
	{
		return false;
	}
	dx = (mx - mox) / ratio;
	dy = (moy - my) / ratio;
	if (rotate && rotateDegree != 0)
	{
		// The picture is turned clockwise about the centre of the screen.
		const double x = dx * rotateCos - dy * rotateSin;
		const double y = dx * rotateSin + dy * rotateCos;
		dx = x;
		dy = y;
	}
	return true;
}

#pragma warning(disable:4244)
void Scaler::DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate)
{
	double ratio = 0;
	if (!PixelSize(ratio))
	{
		return;
	}

	double dx = sx - sox;
	double dy = sy - soy;
	if (rotate && rotateDegree != 0)
	{
		const double x = dx * rotateCos + dy * rotateSin;
		const double y = dy * rotateCos - dx * rotateSin;
		dx = x;
		dy = y;
	}

	*mx = mox + dx * ratio;
	*my = moy - dy * ratio;
}

#pragma warning(disable:4244)
void Scaler::WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate)
{
	double dx = 0;
	double dy = 0;
	if (!FromCentre(mx, my, dx, dy, rotate))
	{
		return;
	}
	*sx = (long)(dx + sox);
	*sy = (long)(dy + soy);
}

#pragma warning(disable:4244)
void Scaler::WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate)
{
	double dx = 0;
	double dy = 0;
	if (!FromCentre(mx, my, dx, dy, rotate))
	{
		return;
	}
	*sx = (float)(dx + sox);
	*sy = (float)(dy + soy);
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

	double centreX = 0;
	double centreY = 0;
	DeviceToWorld((left + right) / 2, (top + bottom) / 2, &centreX, &centreY);

	DeviceToWorld(left, bottom, &xmin, &ymin, false);
	DeviceToWorld(right, top, &xmax, &ymax, false);

	if (rotateDegree != 0)
	{
		const double halfWidth = (xmax - xmin) / 2;
		const double halfHeight = (ymax - ymin) / 2;
		xmin = centreX - halfWidth;
		xmax = centreX + halfWidth;
		ymin = centreY - halfHeight;
		ymax = centreY + halfHeight;
	}

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

	// The actual distance of the X-axis of the map (km): the projection enlarges the ground by
	// its scale factor, taken at the centre of the view.
	double distance = (_mxMax - _mxMin) * EARTH_RADIUS / ProjectionScaleFactor((mxMin + mxMax) / 2, (myMin + myMax) / 2);

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
	// The view keeps its centre; its width on the ground is that of the screen at the scale,
	// enlarged as the projection enlarges the ground there.
	mox = (mxMin + mxMax) / 2;
	moy = (myMin + myMax) / 2;

	const double width = GetScreenWidthKM() * (double)scale / EARTH_RADIUS * ProjectionScaleFactor(mox, moy);
	const double height = (width * (double)GetScreenHeight()) / (double)GetScreenWidth();

	mxMin = mox - width / 2;
	mxMax = mox + width / 2;
	myMin = moy - height / 2;
	myMax = moy + height / 2;

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
	// 0 to 359: the picture turned clockwise about the centre of the screen.
	rotateDegree = ((_value % 360) + 360) % 360;
	rotateSin = std::sin(rotateDegree * DEG2RAD);
	rotateCos = std::cos(rotateDegree * DEG2RAD);
}

void Scaler::Rotate(int value)
{
	SetRotationDegree(rotateDegree + value);
}

double Scaler::NorthOnScreen(double mx, double my) const
{
	return NorthAngle(mx, my) + rotateDegree;
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

	if (IsPolarProjection())
	{
		// x is not a longitude: the world does not repeat.
	}
	else if (mx >= 0)
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

	if (IsPolarProjection())
	{
		// The world does not repeat in x about a pole: nothing to wrap around.
		return;
	}

	if (rotateDegree != 0)
	{
		// Turned, the ends of the world are not columns of the screen: the centre of the view is
		// kept within one turn of the world.
		const double turn = 2 * acos(-1.0);
		const double width = mxMax - mxMin;
		while (mox > turn / 2) mox -= turn;
		while (mox < -turn / 2) mox += turn;
		mxMin = mox - width / 2;
		mxMax = mox + width / 2;
		return;
	}

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

	// About a pole the x-axis has its ends as the y-axis has (on Mercator it goes round).
	if (IsPolarProjection())
	{
		if (mxMin < mxMinLimit)
		{
			mxMin = mxMinLimit;
			mxMax = mxMin + mWidth;
			mox = (mxMin + mxMax) / 2;
		}

		if (mxMax > mxMaxLimit)
		{
			mxMax = mxMaxLimit;
			mxMin = mxMax - mWidth;
			mox = (mxMin + mxMax) / 2;
		}
	}
}

void Scaler::AdjustScreenMap()
{
	AdjustScreenMap_Internal();
}

#pragma warning(disable:4244)
void Scaler::AdjustScreenMap_Internal()
{
	DeviceToWorld(sxMin, syMax, &mxMin, &myMin, false);
	DeviceToWorld(sxMax, syMin, &mxMax, &myMax, false);
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
			// The corners of the screen on the map: the screen is turned clockwise, so they are
			// turned the other way.
			const double rx = x * cosValue - y * sinValue;
			const double ry = x * sinValue + y * cosValue;
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