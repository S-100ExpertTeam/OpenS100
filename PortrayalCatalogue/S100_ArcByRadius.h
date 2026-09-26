#pragma once
#include "S100_VectorPoint.h"
#include "S100_Sector.h"

class S100_ArcByRadius
{
public:
	S100_ArcByRadius();
	virtual ~S100_ArcByRadius();

private:
	S100_VectorPoint center;
	S100_Sector *sector;
	std::string radius;

public:
	void SetCenter(S100_VectorPoint* value);
	S100_VectorPoint* GetCenter();

	void SetSector(S100_Sector* value);
	S100_Sector* GetSector();

	void SetRadius(const std::string& value);
	void SetRadius(const std::wstring& value);
	std::string  GetRadius();
	std::wstring GetRadiusW();

	void GetContents(pugi::xml_node node);

	void DeleteLeftPtr();
};
