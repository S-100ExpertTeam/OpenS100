#include "StdAfx.h"
#include "Scaler.h"
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
}

Scaler::~Scaler(void)
{

}

#pragma warning(disable:4244)
void Scaler::DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate)
{
	if (XYtoSP)
	{
		int _mx = 0;
		int _my = 0;
		XYtoSP(sx, sy, _my, _mx, false, false);
		*mx = _mx;
		*my = _my;

		return;
	}

	double currentRatio = 0;
	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

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
void Scaler::WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate)
{
	if (SPtoXY)
	{
		int _sx = 0;
		int _sy = 0;
		SPtoXY(my, mx, _sx, _sy, false, false);
		*sx = _sx;
		*sy = _sy;

		return;
	}

	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

	double currentRatio = 0;
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

	*sx = (LONG)((mx - mox) / currentRatio + sox);
	*sy = (LONG)((moy - my) / currentRatio + soy);

}

#pragma warning(disable:4244)
void Scaler::WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate)
{
	if (SPtoXY)
	{
		int _sx = 0;
		int _sy = 0;
		SPtoXY(my, mx, _sx, _sy, false, false);
		*sx = _sx;
		*sy = _sy;

		return;
	}

	double currentRatio = 0;
	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

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

void Scaler::SetScreen(RECT *rect)
{
	if (rect == nullptr) return;

	SetScreen(*rect);
}

void Scaler::SetScreen(const RECT& rect)
{
	sxMin = rect.left;
	syMin = rect.top;
	sxMax = rect.right;
	syMax = rect.bottom;

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

	if (rotateDegree)
	{
		Gdiplus::PointF l_u(mxMin - mox, myMax - moy);
		Gdiplus::PointF l_d(mxMin - mox, myMin - moy);
		Gdiplus::PointF r_u(mxMax - mox, myMax - moy);
		Gdiplus::PointF r_d(mxMax - mox, myMin - moy);

		double radian = (rotateDegree)* DEG2RAD;
		double sinValue = sin(radian);
		double cosValue = cos(radian);

		FLOAT tempX = l_u.Y * sinValue + l_u.X * cosValue;
		FLOAT tempY = l_u.Y * cosValue - l_u.X * sinValue;

		l_u.X = tempX;
		l_u.Y = tempY;

		tempX = l_d.Y * sinValue + l_d.X * cosValue;
		tempY = l_d.Y * cosValue - l_d.X * sinValue;

		l_d.X = tempX;
		l_d.Y = tempY;

		tempX = r_u.Y * sinValue + r_u.X * cosValue;
		tempY = r_u.Y * cosValue - r_u.X * sinValue;

		r_u.X = tempX;
		r_u.Y = tempY;

		tempX = r_d.Y * sinValue + r_d.X * cosValue;
		tempY = r_d.Y * cosValue - r_d.X * sinValue;

		r_d.X = tempX;
		r_d.Y = tempY;

		mxMinCalcMBR = l_u.X;
		mxMaxCalcMBR = l_u.X;
		myMinCalcMBR = l_u.Y;
		myMaxCalcMBR = l_u.Y;

		mxMinCalcMBR = l_d.X < mxMinCalcMBR ? l_d.X : mxMinCalcMBR;
		mxMaxCalcMBR = l_d.X > mxMaxCalcMBR ? l_d.X : mxMaxCalcMBR;
		myMinCalcMBR = l_d.Y < myMinCalcMBR ? l_d.Y : myMinCalcMBR;
		myMaxCalcMBR = l_d.Y > myMaxCalcMBR ? l_d.Y : myMaxCalcMBR;

		mxMinCalcMBR = r_u.X < mxMinCalcMBR ? r_u.X : mxMinCalcMBR;
		mxMaxCalcMBR = r_u.X > mxMaxCalcMBR ? r_u.X : mxMaxCalcMBR;
		myMinCalcMBR = r_u.Y < myMinCalcMBR ? r_u.Y : myMinCalcMBR;
		myMaxCalcMBR = r_u.Y > myMaxCalcMBR ? r_u.Y : myMaxCalcMBR;

		mxMinCalcMBR = r_d.X < mxMinCalcMBR ? r_d.X : mxMinCalcMBR;
		mxMaxCalcMBR = r_d.X > mxMaxCalcMBR ? r_d.X : mxMaxCalcMBR;
		myMinCalcMBR = r_d.Y < myMinCalcMBR ? r_d.Y : myMinCalcMBR;
		myMaxCalcMBR = r_d.Y > myMaxCalcMBR ? r_d.Y : myMaxCalcMBR;

		mxMinCalcMBR += mox;
		mxMaxCalcMBR += mox;
		myMinCalcMBR += moy;
		myMaxCalcMBR += moy;
	}
	else
	{
		mxMinCalcMBR = mxMin;
		mxMaxCalcMBR = mxMax;
		myMinCalcMBR = myMin;
		myMaxCalcMBR = myMax;
	}

	UpdateScale();
}

void Scaler::SetMap(MBR mbr)
{
	SetMap(mbr.xmin, mbr.ymin, mbr.xmax, mbr.ymax);
}

void Scaler::SetMap(RECT r)
{
	double xmin = 0;
	double ymin = 0;
	double xmax = 0;
	double ymax = 0;

	DeviceToWorld(r.left, r.bottom, &xmin, &ymin);
	DeviceToWorld(r.right, r.top, &xmax, &ymax);

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

CString Scaler::GetFormatedScale()
{
	CString str;
	double Scale = GetCurrentScale();

	CString newStr, strComma;

	strComma.Format(_T(","));
	str.Format(_T("%.lf"), Scale);

	int addedLength;

	if ((str.GetLength() % 3) == 0) {
		addedLength = (str.GetLength() / 3) - 1;
	}
	else {
		addedLength = (str.GetLength() / 3);
	}

	newStr = str;

	for (int k = 0; k < addedLength; k++) {
		newStr.AppendChar('a');
	}

	int cnt = 0;

	int i = str.GetLength() - 1;
	int j = newStr.GetLength() - 1;

	for (; i >= 0;) {
		newStr.SetAt(j, str.GetAt(i));
		j--;
		i--;
		cnt++;

		if ((cnt == 3) && (j >= 0)) {
			newStr.SetAt(j, strComma.GetAt(0));
			j--;
			cnt = 0;
		}
	}

	str.Format(_T("1 : "));
	str.AppendFormat(newStr);


	return str;
}

double Scaler::GetRotationDegree()
{
	return rotateDegree;
}

void Scaler::SetRotationDegree(double _value)
{
	rotateDegree = _value;
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
	double oldMx, oldMy;

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

	LONG sx1 = 0, sy1 = 0;     // On the right side of the map.
	LONG sx2 = 0, sy2 = 0;	   // The left side of the map.

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
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
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
	MBR mbr(mxMin, myMin, mxMax, myMax);

	return mbr;
}

MBR Scaler::GetMapCalcMBR()
{
	MBR mbr(mxMinCalcMBR, myMinCalcMBR, mxMaxCalcMBR, myMaxCalcMBR);

	return mbr;
}

double Scaler::GetScreenWidthKM()
{
	int width = GetScreenWidth();
	double widthCM = (double)width / (double)DOTPERCM;

	return (double)widthCM * pow(10.0, -5);
}

#pragma warning(disable:4244)
int Scaler::GetScreenWidth()
{
	return sxMax - sxMin + 1;
}

int Scaler::GetScreenHeight()
{
	return syMax - syMin + 1;
}

CRect Scaler::GetScreenRect()
{
	return CRect(0, 0, GetScreenWidth(), GetScreenHeight());
}





#pragma warning(disable:4244)
D2D1_RECT_F Scaler::GetD2Rect()
{
	return D2D1::RectF(0, 0, GetScreenWidth(), GetScreenHeight());
}

bool Scaler::PtInMap(double _x, double _y)
{
	if ((_x >= mxMin) && (_x <= mxMax) && (_y >= myMin) && (_y <= myMax)) 
	{
		return true;
	}

	return false;
}

#pragma warning(disable:4244)
D2D1::Matrix3x2F Scaler::GetMatrix()
{
	D2D1::Matrix3x2F result = D2D1::Matrix3x2F::Identity();

	auto translation1 = D2D1::Matrix3x2F::Translation(-mox, moy);
	double ratio = (sxMax - sxMin) / (mxMax - mxMin);
	auto scale = D2D1::Matrix3x2F::Scale(ratio, ratio);
	auto translation2 = D2D1::Matrix3x2F::Translation(sox, soy);

	result = translation1 * scale * translation2;

	return result;
}

D2D1::Matrix3x2F Scaler::GetInverseMatrix()
{
	auto matrix = GetMatrix();
	matrix.Invert();
	return matrix;
}

CRect Scaler::WGS84MBRtoCRect(MBR& mbr)
{
	CRect rect;
	WorldToDevice(mbr.GetXMin(), mbr.GetYMin(), &rect.left, &rect.bottom);
	WorldToDevice(mbr.GetXMax(), mbr.GetYMax(), &rect.right, &rect.top);
	return rect;
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

long long Scaler::GetCOMF()
{
	return COMF;
}