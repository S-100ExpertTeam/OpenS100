#pragma once
#include "Field.h"

class F_DSSI : Field
{
public:
	F_DSSI();
	virtual ~F_DSSI();

public:
	double m_dcox = 0.0;
	double m_dcoy = 0.0;
	double m_dcoz = 0.0;
	int m_cmfx = 10000000;
	int m_cmfy = 10000000;
	int m_cmfz = 100;

	// Number of Information Type Records
	int m_noir;

	// Number of Point records
	int m_nopn;

	// Number of Multi Point records
	int m_nomn;

	// Number of Curve records
	int m_nocn;

	// Number of Composite Curve records
	int m_noxn;

	// Number of Surface records
	int m_nosn;

	// Number of Feature Type Records
	int m_nofr;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();

	int GetNumberOfInformationTypeRecords();
	void SetNumberOfInformationTypeRecords(int value);

	int GetNumberOfPointRecords();
	void SetNumberOfPointRecords(int value);

	int GetNumberOfMultiPointRecords();
	void SetNumberOfMultiPointRecords(int value);

	int GetNumberOfCurveRecords();
	void SetNumberOfCurveRecords(int value);

	int GetNumberOfCompositeCurveRecords();
	void SetNumberOfCompositeCurveRecords(int value);

	int GetNumberOfSurfaceRecords();
	void SetNumberOfSurfaceRecords(int value);

	int GetNumberOfFeatureTypeRecords();
	void SetNumberOfFeatureTypeRecords(int value);
};