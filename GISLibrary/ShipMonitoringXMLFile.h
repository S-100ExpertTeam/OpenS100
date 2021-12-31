#pragma once
#include "SpatialObject.h"

#include <vector>

class ShipMonitoringXMLFile :
	public SpatialObject
{
public:
	ShipMonitoringXMLFile();
	virtual ~ShipMonitoringXMLFile();

public:
	unsigned int m_nTrackRecords;
	struct Record
	{
		COleDateTime time;
		double lat;
		double lon;
	};
	std::vector<Record> m_records;

public:
	bool Open(CString _filepath);
	void Draw(CDC* pDC, Scaler* scaler, double offset = 0);
};