#pragma once
#include "S100Layer.h"

class PortrayalCatalogue;
class FeatureCatalogue;
class S101Cell;
class S101Layer : public S100Layer
{
public:
	S101Layer(FeatureCatalogue* fc = nullptr, PortrayalCatalogue* pc = nullptr);
	virtual ~S101Layer();

public:
	virtual bool Open(CString _filepath) override;
	void SetSpatialObject(S101Cell* cell);
	void Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler);

	std::wstring GetChartName();
	std::wstring GetEditionNumberAsWstring();
	std::wstring GetUpdateNumberAsWstring();
	std::wstring GetIssueDateAsWstring();
};