#pragma once

#include "../GeoMetryLibrary/Geometry.h"

#include "Enum_SGeometryType.h"
#include "GF_InformationType.h"

#include <vector>
#include <string>

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
	std::vector<GF::InformationType*> additionalInformation;

public:
	void AddInformationType(GF::InformationType* info);
	int GetInformationTypeCount();
	GF::InformationType* GetInformationType(int index);

public:
	virtual SGeometryType GetType() { return SGeometryType::none; }
};
