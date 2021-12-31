#include "StdAfx.h"
#include "ENCGeometry.h"
#include "GeoPoint.h"

#include "..\\S100_SVG_D2D1_DLL\\SVG.h"
#include "..\\S100_SVG_D2D1_DLL\\SVGManager.h"

#include <cmath>
				
CGeoObjectMap	*ENCGeometry::pObject = NULL;
CAttributeMap	*ENCGeometry::pAttribute = NULL;
CColorMap		*ENCGeometry::pColor = NULL;
CSymbolMap		*ENCGeometry::pSymbol = NULL;


ENCGeometry::ENCGeometry(void) /*: pen(Color(0, 0, 0), 1)*/
{
	m_simplePLIndex = -1;
	m_paperPLIndex = -1;
	m_pFeature = NULL;
	m_svgSymbol = NULL;
}

ENCGeometry::~ENCGeometry(void)
{
}

void ENCGeometry::SetPLIndex(int simple, int paper)
{
	m_simplePLIndex = simple;
	m_paperPLIndex = paper;
}


void ENCGeometry::SetScaleMin(unsigned value)
{
	m_scaleMin = value;
}