#include "stdafx.h"
#include "S100DrawingUnitPolygon.h"

S100DrawingUnitPolygon::S100DrawingUnitPolygon()
{
	color = D2D1::ColorF(D2D1::ColorF::Crimson);
	pGeometry = nullptr;
	transparency = 0;
}

S100DrawingUnitPolygon::~S100DrawingUnitPolygon()
{
	SafeRelease(&pGeometry);
}