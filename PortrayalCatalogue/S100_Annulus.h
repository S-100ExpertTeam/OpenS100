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
	std::wstring *innerRadius;
	std::wstring outerRadius;
	S100_Sector *sector;

public:
	void GetContents(pugi::xml_node node);

	void SetPoint(S100_VectorPoint* value);
	S100_VectorPoint* GetPoint();

	void SetInnerRadius(std::wstring* value);
	std::wstring* GetinnerRadius();

	void SetOuterRadius(std::wstring& value);
	std::wstring GetOuterRadius();

	void SetSector(S100_Sector* value);
	S100_Sector* GetSector();
};