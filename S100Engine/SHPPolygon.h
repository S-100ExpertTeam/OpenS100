#pragma once
#include "SHPPoint.h"

class SHPPolygon
{
public:
	SHPPolygon();
	virtual ~SHPPolygon();

public:
	int numParts = 0;
	int numPoints = 0;
	int* parts = nullptr;
	SHPPoint* points = nullptr;

public:
	void Read(int index, CFile& fdSHP, CFile& fdSHX);
	int GetNumParts();
	int GetNumPoints();
	int GetNumPointsPerPart(int index);
	float GetX(int index);
	float GetY(int index);
	void SwapWord(int _length, void *_pData);
};