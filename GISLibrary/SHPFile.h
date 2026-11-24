#pragma once
#include "DeviceTypes.h"
#include "SpatialObject.h"

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
	bool Open(const std::wstring& _filepath) override;
	bool OpenMetadata(const std::wstring& _filepath) override;

	void Draw(GISLibrary::ViewFrame& frame, Scaler *scaler, double offset = 0) override;

	/*
	** Description: Translate Big Endian and Little Endian into each other.
	*/
	void SwapWord(int _length, void *_pData);
};