#pragma once
#include "EnumTypeOfDrawingInstruction.h"
#include "FeatureReference.h"
#include "SpatialReference.h"

class DrawingInstruction
{
public:
	DrawingInstruction();
	virtual ~DrawingInstruction();

public:
	CString m_strViewingGroup;
	CString m_strDisplayPlane;
	int     m_nDrawingPriority;
	int     m_nScaleMinimum;    // If it's 0, don't use it.
	int     m_nScaleMaximum;    // If it's 0, don't use it.
	FeatureReference *m_pFeatureReference;
	SpatialReference *m_pSpatialReference;
	TypeOfDrawingInstruction m_nTypeOfDrawingInstruction;
	int m_nRefGeometryType;
	int m_nRefGeometryID;
};