#include "stdafx.h"
#include "S100DrawingUnitPolyLine.h"

S100DrawingUnitPolyLine::S100DrawingUnitPolyLine()
{
	color = D2D1::ColorF(D2D1::ColorF::Crimson);
	width = 1.0;
	pGeometry = nullptr;
	hasDash = false;
	lineStyleReference = _T("");
}

S100DrawingUnitPolyLine::~S100DrawingUnitPolyLine()
{

}