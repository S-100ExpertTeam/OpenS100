#pragma once
#include "spatialobject.h"
#include "ExchangeCatalogue.h"

#include <memory>

class Geometry;
class Scaler;

class S100ExchangeCatalogue :
	public SpatialObject
{
public:
	S100ExchangeCatalogue();
	virtual ~S100ExchangeCatalogue();

public:
	unsigned int m_nRecords;					// The number of records
	unsigned int m_nShapeType;					// Shape Type
	Geometry** m_pSHPObject;
	std::shared_prt<ExchangeCatalogue> 

public:
	/*
	** Description: Read the file and store the data in the layer.
	*/
	bool Open(CString _filepath) override;

	void Draw(HDC& hDC, Scaler* scaler, double offset = 0) override;
}