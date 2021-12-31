#include "stdafx.h"
#include "ClipperUtil.h"

#include "..\\GeoMetryLibrary\\GeoPoint.h"
#include "..\\GeoMetryLibrary\\Scaler.h"
#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

ClipperLib::PolyTree* ClipperUtil::ClipPolyline(GeoPoint* _p, int _cnt, Scaler* scaler)
{
	ClipperLib::Path  polyline, view;
	ClipperLib::PolyTree* result = new ClipperLib::PolyTree();
	ClipperLib::Clipper clipper;

	ClipperLib::IntPoint tmp;

	auto COMF = scaler->GetCOMF();

	tmp.X = (ClipperLib::cInt)(scaler->GetMapXMin() * COMF);
	tmp.Y = (ClipperLib::cInt)(scaler->GetMapYMax() * COMF);
	view.push_back(tmp);
	tmp.X = (ClipperLib::cInt)(scaler->GetMapXMax() * COMF);
	tmp.Y = (ClipperLib::cInt)(scaler->GetMapYMax() * COMF);
	view.push_back(tmp);
	tmp.X = (ClipperLib::cInt)(scaler->GetMapXMax() * COMF);
	tmp.Y = (ClipperLib::cInt)(scaler->GetMapYMin() * COMF);
	view.push_back(tmp);
	tmp.X = (ClipperLib::cInt)(scaler->GetMapXMin() * COMF);
	tmp.Y = (ClipperLib::cInt)(scaler->GetMapYMin() * COMF);
	view.push_back(tmp);
	view.push_back(*view.begin());

	for (int i = 0; i < _cnt; i++) {
		tmp.X = (ClipperLib::cInt)(_p[i].x * COMF);
		tmp.Y = (ClipperLib::cInt)(_p[i].y * COMF);
		polyline.push_back(tmp);
	}

	clipper.AddPath(polyline, ClipperLib::ptSubject, false);
	clipper.AddPath(view, ClipperLib::ptClip, true);
	clipper.Execute(ClipperLib::ctIntersection, *result);

	return result;
}