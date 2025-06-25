#pragma once

#include "Layer.h"
#include "DrawingSet.h"

#include <string>
#include <set>

class Catalog;
class PortrayalCatalogue;
class S100SpatialObject;
class FeatureCatalogue;

class S100Layer : public Layer
{
public:
	S100Layer();
	S100Layer(int productNumber);
	S100Layer(FeatureCatalogue* fc, PortrayalCatalogue* pc);
	virtual ~S100Layer();

protected:
	FeatureCatalogue* featureCatalogue = nullptr;
	PortrayalCatalogue* portrayalCatalogue = nullptr;
	bool hasIndividualFC = false;
	bool hasIndividualPC = false;
	int productNumber = 100;

public: // for draw
	std::set<int> drawingPriority;
	DrawingSet drawingSet;

public:
	bool Open(CString _filepath, D2D1Resources* d2d1, LayerManager* lm = nullptr) override;
	void Draw(HDC& hDC, Scaler* scaler, double offset = 0) override;
	void SetFeatureCatalog(FeatureCatalogue* value);
	FeatureCatalogue* GetFeatureCatalog();
	FeatureCatalogue* GetFC();
	void SetPC(PortrayalCatalogue* value);
	PortrayalCatalogue* GetPC();
	void SetIndividualFC(bool value);
	bool GetIndividualFC();
	void SetIndividualPC(bool value);
	bool GetIndividualPC();
	bool OpenFC(CString path);
	bool OpenPC(CString path, D2D1Resources* d2d1);
	void DeleteCatalog();
	void DrawInfo(HDC& hDC, Scaler* scaler);
	S100SpatialObject* GetS100SpatialObject();

	int GetProductNumber();
	void SetProductNumber(int value);

	// value : S-101, S-102, S-111 etc.
	void SetProductNumber(std::wstring value);

	void InitDraw();

	void BuildPortrayalCatalogue();
};