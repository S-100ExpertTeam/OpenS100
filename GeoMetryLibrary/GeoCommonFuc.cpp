#include "StdAfx.h"
#include "GeoCommonFuc.h"
#include "MBR.h"
#include "ENCCommon.h"

#include <cmath>

//projection
void projection(double &x, double &y)
{
	x *= ex;
	
	if (y <= limit)
	{
		y = limit;
	}
	else if (y >= -limit)
	{
		y = -limit;
	}

	y *= ex;			
	//y = log(tan((y / 2.0) + M_PI_D4));
}


void projection(float &x, float &y)
{
	double _x = x;
	double _y = y;
	projection(_x, _y);
	x = (float)_x;
	y = (float)_y;
}


void projection(MBR& mbr)
{
	projection(mbr.xmin, mbr.ymin);
	projection(mbr.xmax, mbr.ymax);
}


void inverseProjection(double& x, double& y)
{
	double pi = M_PI;
	double RADDEG = (180.0 / pi);

	x *= RADDEG;
	//y = 2 * (atan(exp(y)) - (pi / 4.0));
	y *= RADDEG;
}


void inverseProjection(MBR& mbr)
{
	inverseProjection(mbr.xmin, mbr.ymin);
	inverseProjection(mbr.xmax, mbr.ymax);
}


unsigned getClippingLocation(const MBR *mbr, const double &x, const double &y)
{
	if(mbr == NULL)
		return 0;

	unsigned loc = 0;
	if(x < mbr->xmin)
	{
		if(y < mbr->ymin)
		{
			loc = 1;
		}
		else if(y > mbr->ymin && y < mbr->ymax)
		{
			loc = 4;
		}
		else if(y > mbr->ymax)
		{
			loc = 7;
		}
	}
	else if(x > mbr->xmin && x < mbr->xmax)
	{
		if(y < mbr->ymin)
		{
			loc = 2;
		}
		else if(y > mbr->ymax)
		{
			loc = 8;
		}
	}
	else if(x > mbr->xmax)
	{
		if(y < mbr->ymin)
		{
			loc = 3;
		}
		else if(y > mbr->ymin && y < mbr->ymax)
		{
			loc = 6;
		}
		else if(y > mbr->ymax)
		{
			loc = 9;
		}
	}
	return loc;
}