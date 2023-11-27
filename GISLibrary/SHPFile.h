#pragma once
#include "spatialobject.h"

class Geometry;
class Scaler;

class SHPFile :
	public SpatialObject
{
public:
	SHPFile();
	virtual ~SHPFile();

public:
	unsigned int m_nRecords;					// The number of records
	unsigned int m_nShapeType;					// Shape Type
	Geometry **m_pSHPObject;

public:
	/*
	** Description: Read the file and store the data in the layer.
	*/
	bool Open(CString _filepath);

	void Draw(HDC &hDC, Scaler *scaler, double offset = 0);

	/*
	** Description: Translate Big Endian and Little Endian into each other.
	*/
	void SwapWord(int _length, void *_pData);
};