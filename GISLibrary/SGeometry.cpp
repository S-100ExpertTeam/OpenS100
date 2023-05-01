#include "stdafx.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
// It also has a CPoint arrangement to minimize memory allocation and recovery.
POINT   *SGeometry::viewPoints = new POINT[10];

SGeometry::SGeometry() 
{

}

SGeometry::~SGeometry()
{

}