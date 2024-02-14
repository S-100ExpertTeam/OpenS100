#pragma once
#include "spatialobject.h"
#include "ExchangeCatalogue.h"


#include <memory>

using namespace S100;

class Geometry;
class Scaler;
class Inventory;
class InventoryItem;
class ScaleRange;


class S100ExchangeCatalogue :
	public SpatialObject
{
public:
	S100ExchangeCatalogue(Scaler* scaler, CatalogManager* cm, D2D1Resources* d2d1, LayerManager* parent);
	virtual ~S100ExchangeCatalogue();

public:
	bool m_IsLegacy;

	unsigned int m_nRecords;					// The number of records
	unsigned int m_nShapeType;					// Shape Type
	shared_ptr<ExchangeCatalogue>  m_DataPtr;	//

	std::vector<shared_ptr<Inventory>> m_vecInventory;

	LayerManager* m_pLayerManager;

	LayerManager* m_pParentLayerManager;

	


public:
	/*
	** Description: Read the file and store the data in the layer.
	*/
	bool Open(CString _filepath) override;

	void Draw(HDC& hDC, Scaler* scaler, double offset) override;

	void DrawEx(HDC& hDC, Scaler* scaler, double offset);

	void DrawFiles(HDC& hDC, Scaler* scaler, double offset);
	
	bool containsFile(const std::vector<std::shared_ptr<InventoryItem>>& files, const CString& targetFile);

	bool CompareByFileName(const CString& a, const CString& b);

	bool CompareByLayer(const Layer* a, const Layer* b);

	CString FixFileName(string str);

	CString FixLegacyFileName(string str);
};