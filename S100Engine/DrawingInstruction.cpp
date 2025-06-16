#include "stdafx.h"
#include "DrawingInstruction.h"

DrawingInstruction::DrawingInstruction()
{
	m_nDrawingPriority = 0;
	m_nScaleMinimum = 0;
	m_nScaleMaximum = 0;

	m_pFeatureReference = nullptr;
	m_pSpatialReference = nullptr;

	m_nRefGeometryType = -1;
	m_nRefGeometryID = -1;
}

DrawingInstruction::~DrawingInstruction()
{
	delete m_pFeatureReference;
	delete m_pSpatialReference;
}
