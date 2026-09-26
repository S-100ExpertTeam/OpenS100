#pragma once
#include "S100_VectorPoint.h"
#include "S100_Sector.h"

class S100_Annulus
{
public:
	S100_Annulus();
	virtual ~S100_Annulus();

private:
	S100_VectorPoint point;
	std::string* innerRadius = nullptr;
	std::string  outerRadius;
	S100_Sector* sector = nullptr;

public:
	void GetContents(pugi::xml_node node);

	void SetPoint(S100_VectorPoint* value);
	S100_VectorPoint* GetPoint();

	void SetInnerRadius(std::string* value);
	std::string* GetinnerRadius();

	void SetOuterRadius(const std::string& value);
	void SetOuterRadius(const std::wstring& value);
	std::string  GetOuterRadius();
	std::wstring GetOuterRadiusW();

	void SetSector(S100_Sector* value);
	S100_Sector* GetSector();
};
