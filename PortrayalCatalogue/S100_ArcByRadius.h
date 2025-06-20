#pragma once
#include "S100_VectorPoint.h"
#include "S100_Sector.h"

class S100_ArcByRadius
{
public:
	S100_ArcByRadius();
	virtual ~S100_ArcByRadius();

private:
	S100_VectorPoint* center;
	S100_Sector* sector;
	std::wstring radius;

public:
	void SetCenter(S100_VectorPoint* value);
	void SetCenter(std::wstring& x, std::wstring& y);
	S100_VectorPoint* GetCenter();

	void SetSector(S100_Sector* value);
	void SetSector(std::wstring& angle, std::wstring& distance);
	S100_Sector* GetSector();

	void SetRadius(std::wstring& value);
	std::wstring GetRadius();

	void GetContents(pugi::xml_node node);

	void DeleteLeftPtr();
};