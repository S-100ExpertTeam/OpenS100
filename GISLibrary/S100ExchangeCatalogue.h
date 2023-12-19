#pragma once
#include "spatialobject.h"
#include "ExchangeCatalogue.h"


#include <memory>

using namespace S100;

class Geometry;
class Scaler;
class Inventory;
class ScaleRange;


class S100ExchangeCatalogue :
	public SpatialObject
{
public:
	S100ExchangeCatalogue();
	virtual ~S100ExchangeCatalogue();

public:
	unsigned int m_nRecords;					// The number of records
	unsigned int m_nShapeType;					// Shape Type
	shared_ptr<ExchangeCatalogue>  m_DataPtr;	//

	std::vector<shared_ptr<Inventory>> m_vecInventory;


public:
	/*
	** Description: Read the file and store the data in the layer.
	*/
	bool Open(CString _filepath) override;

	void Draw(HDC& hDC, Scaler* scaler, double offset) override;

	void DrawEx(HDC& hDC, Scaler* scaler, double offset);

	void DrawFiles(HDC& hDC, Scaler* scaler, double offset);
};