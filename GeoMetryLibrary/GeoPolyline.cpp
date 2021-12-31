#include "StdAfx.h"
#include "GeoPolyline.h"
#include "GeoPoint.h"
#include "Scaler.h"
#include "GeoCommonFuc.h"

GeoPolyline::GeoPolyline()
{

}

GeoPolyline::~GeoPolyline()
{
	delete[] Parts;
	Parts = nullptr;

	delete[] Points;
	Points = nullptr;
	
	delete[] CPoints;
	CPoints = nullptr;
}

void GeoPolyline::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
{
	// Change the map coordinate system to a screen coordinate system and store it in CPoint.
	int viewPointNum = 0;
	int partsIndex = 1;
	int lastPointIndex = -1;

	unsigned preLoc = -1, curLoc = 0;

	for (int i = 0; i < NumPoints; i++) 
	{
		/////////////////////////////
		// Clipping is performed compared to the MBR of the screen, and coordinates after clipping are obtained.
		/////////////////////////////
		MBR _mbr;
		_mbr.xmin = scaler->mxMin - 0.2;
		_mbr.xmax = scaler->mxMax + 0.2;
		_mbr.ymin = scaler->myMin - 0.2;
		_mbr.ymax = scaler->myMax + 0.2;
		curLoc = getClippingLocation(&_mbr, Points[i].x + offset, Points[i].y);

		if (curLoc == 0 || curLoc != preLoc)
		{
			if (preLoc != -1 && preLoc != 0 && lastPointIndex != i - 1)
			{
				/////////////////////////////
				// Previous point
				++viewPointNum;
				// The clipped coordinates are composed of screen coordinates. (Unimplemented)
				scaler->WorldToDevice(
					Points[i - 1].x + offset, 
					Points[i - 1].y,
					&CPoints[viewPointNum - 1].x, 
					&CPoints[viewPointNum - 1].y);
				/////////////////////////////
			}
			/////////////////////////////
			// Current point
			++viewPointNum;
			// The clipped coordinates are composed of screen coordinates. (Unimplemented)
			scaler->WorldToDevice(
				Points[i].x + offset, 
				Points[i].y,
				&CPoints[viewPointNum - 1].x, 
				&CPoints[viewPointNum - 1].y);
			/////////////////////////////

			/////////////////////////////
			// A device to avoid overlapping coordinates that you entered most recently.
			lastPointIndex = i;
			/////////////////////////////
		}

		preLoc = curLoc;

		if (i == Parts[partsIndex] - 1 || i == NumPoints - 1)
		{
			if (viewPointNum) 
			{
				::Polyline(hDC, &CPoints[0], viewPointNum);
			}
			preLoc = -1;
			++partsIndex;
			viewPointNum = 0;
		}
	}
}