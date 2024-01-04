#include "stdafx.h"
#include "MBR.h"

MBR::MBR(void)
{
}

MBR::MBR(const MBR& other)
{
	xmin = other.xmin;
	ymin = other.ymin;
	xmax = other.xmax;
	ymax = other.ymax;
}

MBR::MBR(double _xmin, double _ymin, double _xmax, double _ymax)
{
	xmin = _xmin;
	ymin = _ymin;
	xmax = _xmax;
	ymax = _ymax;
}

MBR::~MBR(void)
{
}

MBR MBR::operator=(const MBR& other)
{
	xmin = other.xmin;
	ymin = other.ymin;
	xmax = other.xmax;
	ymax = other.ymax;

	return *this;
}

bool MBR::operator==(const MBR& other)
{
	if ((xmin == other.xmin) &&
		(ymin == other.ymin) &&
		(xmax == other.xmax) &&
		(ymax == other.ymax))
		return true;

	return false;
}

void MBR::SetXMin(double value)
{
	xmin = value;
}

double MBR::GetXMin()
{
	return xmin;
}

void MBR::SetYMin(double value)
{
	ymin = value;
}

double MBR::GetYMin()
{
	return ymin;
}

void MBR::SetXMax(double value)
{
	xmax = value;
}

double MBR::GetXMax()
{
	return xmax;
}

void MBR::SetYMax(double value)
{
	ymax = value;
}

double MBR::GetYMax()
{
	return ymax;
}

void MBR::SetMBR(MBR mbr)
{
	xmin = mbr.xmin;
	ymin = mbr.ymin;
	xmax = mbr.xmax;
	ymax = mbr.ymax;
}

void MBR::SetMBR(double _xmin, double _ymin, double _xmax, double _ymax)
{
	xmin = _xmin;
	ymin = _ymin;
	xmax = _xmax;
	ymax = _ymax;
}

double MBR::GetWidth()
{
	return xmax - xmin;
}

double MBR::GetHeight()
{
	return ymax - ymin;
}

double MBR::GetCenterX()
{
	return (xmin + xmax) / 2.0;
}

double MBR::GetCenterY()
{
	return (ymin + ymax) / 2.0;
}

void MBR::ReMBR(MBR newMBR)
{
	if (xmin > newMBR.xmin)
	{
		xmin = newMBR.xmin;
	}

	if (ymin > newMBR.ymin) 
	{
		ymin = newMBR.ymin;
	}

	if (xmax < newMBR.xmax)
	{
		xmax = newMBR.xmax;
	}

	if (ymax < newMBR.ymax) 
	{
		ymax = newMBR.ymax;
	}
}

void MBR::InitMBR()
{	
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
}

void MBR::CalcMBR(double x, double y)
{
	if (xmin > x)
	{
		xmin = x;
	}
	if (ymin > y) 
	{
		ymin = y;
	}
	if (xmax < x) 
	{
		xmax = x;
	}
	if (ymax < y)
	{
		ymax = y;
	}
}

void MBR::CalcMBR(MBR& mbr)
{
	CalcMBR(mbr.GetXMin(), mbr.GetYMin());
	CalcMBR(mbr.GetXMin(), mbr.GetYMax());
	CalcMBR(mbr.GetXMax(), mbr.GetYMax());
	CalcMBR(mbr.GetXMax(), mbr.GetYMin());
}

bool MBR::Intersects(MBR _mbr)
{
	return !((_mbr.xmax < xmin) || (_mbr.ymax < ymin) || (_mbr.xmin > xmax) || (_mbr.ymin > ymax));
}

bool MBR::Overlap(MBR _mbr)
{
	if ((_mbr.ymin >= ymin && _mbr.ymin <= ymax) ||
		(_mbr.ymax >= ymin && _mbr.ymax <= ymax) ||
		(_mbr.ymin <= ymin && _mbr.ymax >= ymax)
		)
	{
		if ((_mbr.xmin >= xmin && _mbr.xmin <= xmax) ||
			(_mbr.xmax >= xmin && _mbr.xmax <= xmax) ||
			(_mbr.xmin <= xmin && _mbr.xmax >= xmax)
			)
		{
			return true;
		}
	}
	return false;
}

bool MBR::CheckIntersects(MBR _mbr1, MBR _mbr2)
{
	return !((_mbr2.xmax < _mbr1.xmin) || (_mbr2.ymax < _mbr1.ymin) || (_mbr2.xmin > _mbr1.xmax) || (_mbr2.ymin > _mbr1.ymax));
}

bool MBR::CheckOverlap(MBR screenMBR, MBR objMBR)
{
	if ((objMBR.ymin >= screenMBR.ymin && objMBR.ymin <= screenMBR.ymax) ||
		(objMBR.ymax >= screenMBR.ymin && objMBR.ymax <= screenMBR.ymax) ||
		(objMBR.ymin <= screenMBR.ymin && objMBR.ymax >= screenMBR.ymax)
		)
	{
		if ((objMBR.xmin >= screenMBR.xmin && objMBR.xmin <= screenMBR.xmax) ||
			(objMBR.xmax >= screenMBR.xmin && objMBR.xmax <= screenMBR.xmax) ||
			(objMBR.xmin <= screenMBR.xmin && objMBR.xmax >= screenMBR.xmax)
			)
		{
			return true;
		}
	}
	return false;
}

bool MBR::operator== (MBR& _mbr)
{
	if ((xmin == _mbr.xmin) && (xmax == _mbr.xmax) && (ymin == _mbr.ymin) && (ymax == _mbr.ymax))
	{
		return true;
	}
	return false;
}

bool MBR::IntersectMBR(const MBR& _mbr1, const MBR& _mbr2, MBR& _intersectMBR)
{
	if (_mbr1.xmin > _mbr2.xmin)
	{
		_intersectMBR.xmin = _mbr1.xmin;
	}
	else
	{
		_intersectMBR.xmin = _mbr2.xmin;
	}

	if (_mbr1.xmax > _mbr2.xmax)
	{
		_intersectMBR.xmax = _mbr2.xmax;
	}
	else
	{
		_intersectMBR.xmax = _mbr1.xmax;
	}

	if (_intersectMBR.xmax < _intersectMBR.xmin)
	{
		return false;
	}

	if (_mbr1.ymin > _mbr2.ymin)
	{
		_intersectMBR.ymin = _mbr1.ymin;
	}
	else
	{
		_intersectMBR.ymin = _mbr2.ymin;
	}

	if (_mbr1.ymax > _mbr2.ymax)
	{
		_intersectMBR.ymax = _mbr2.ymax;
	}
	else
	{
		_intersectMBR.ymax = _mbr1.ymax;
	}

	if (_intersectMBR.ymax < _intersectMBR.ymin)
	{
		return false;
	}
	return true;
}

bool MBR::PtInMBR(double x, double y)
{
	if ((xmin <= x) && (x <= xmax) && (ymin <= y) && (y <= ymax))
	{
		return true;
	}
	return false;
}

void MBR::Scale(double scale)
{
	double marginX = GetWidth() * scale;
	double marginY = GetHeight() * scale;

	SetXMin(GetXMin() - marginX);
	SetYMin(GetYMin() - marginY);
	SetXMax(GetXMax() + marginX);
	SetYMax(GetYMax() + marginY);
}