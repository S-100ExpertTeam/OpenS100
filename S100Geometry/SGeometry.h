#pragma once

#include "../GeoMetryLibrary/Geometry.h"

#include "Enum_SGeometryType.h"

#include <unordered_map>

class SSurface;
class Symbol;
class R_FeatureRecord;
class SENC_SymbolFill;

class SGeometry : public Geometry
{
public:
	SGeometry();
	virtual ~SGeometry();

public:
	static int sizeOfPoint;

	// It also has a CPoint arrangement to minimize memory allocation and recovery.
	static POINT* viewPoints;

public:
	// 1 : Point 
	// 2 : SCompositeCurve
	// 3 : Surface
	// 4 : Multi Point (Sounding)
	// 5 : SCurve
	virtual SGeometryType GetType() { return SGeometryType::none; }
};
