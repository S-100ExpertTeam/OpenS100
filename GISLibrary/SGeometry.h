#pragma once

#include "../GeoMetryLibrary/Geometry.h"

#include "Enum_SGeometryType.h"
#include "IF_InformationType.h"

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
	std::vector<S100Interface::InformationType*> additionalInformation;

public:
	void AddInformationType(S100Interface::InformationType* info);
	int GetInformationTypeCount();
	S100Interface::InformationType* GetInformationType(int index);

public:
	virtual SGeometryType GetType() { return SGeometryType::none; }
};
