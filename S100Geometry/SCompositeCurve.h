#pragma once
#include "SGeometry.h"
#include "SCurveHasOrient.h"

class SCompositeCurve : public SGeometry
{
public:
	SCompositeCurve();
	virtual ~SCompositeCurve();

public:
	std::list<SCurveHasOrient> m_listCurveLink;

public:
	void SetMBR();

	void CreateD2Geometry(ID2D1Factory1* factory);
	std::list<ID2D1PathGeometry*> GetD2Geometry();
	std::list<ID2D1PathGeometry*> GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler);

	bool ImportFromWkb(char* value, int size);
	bool ExportToWkb(char** value, int* size);

	void AddCurve(SCurve* curve, bool masking = false);
};