#include "stdafx.h"
#include "ShipMonitoringXMLFile.h"

ShipMonitoringXMLFile::ShipMonitoringXMLFile()
{
	m_FileType = S100_FileType::FILE_ETC;
}

ShipMonitoringXMLFile::~ShipMonitoringXMLFile()
{

}

bool ShipMonitoringXMLFile::Open(CString _filePath)
{
	MSXML2::IXMLDOMDocument2Ptr pDoc;
	pDoc.CreateInstance(__uuidof(DOMDocument));
	if (!pDoc->load((_variant_t)_filePath))
	{
		AfxMessageBox(_filePath + _T(" 을 여는데 실패했습니다."));
		return false;
	}

	MSXML2::IXMLDOMNodeListPtr records;

	records = pDoc->selectNodes(L"//Record");

	double minLat = 0;
	double maxLat = 0;
	double minLon = 0;
	double maxLon = 0;

	for (int i = 0; i < records->Getlength(); i++)
	{
		Record record;
		record.lon = _variant_t(records->Getitem(i)->firstChild->text);
		record.lat = _variant_t(records->Getitem(i)->firstChild->nextSibling->text);
		int year = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->firstChild->text);
		int month = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->firstChild->nextSibling->text);
		int day = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->firstChild->nextSibling->nextSibling->text);
		int hour = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->nextSibling->firstChild->text);
		int minute = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->nextSibling->firstChild->nextSibling->text);
		int second = _variant_t(records->Getitem(i)->firstChild->nextSibling->nextSibling->firstChild->nextSibling->firstChild->nextSibling->nextSibling->text);	
		record.time.SetDateTime(year, month, day, hour, minute, second);
		m_records.push_back(record);

		projection(record.lon, record.lat);

		if (i == 0)
		{
			minLat = record.lat;
			minLon = record.lon;
			maxLat = record.lat;
			maxLon = record.lon;
		}
		else
		{
			if (minLat > record.lat)
			{
				minLat = record.lat;
			}
			else if (maxLat < record.lat)
			{
				maxLat = record.lat;
			}

			if (minLon > record.lon)
			{
				minLon = record.lon;
			}
			else if (maxLon < record.lon)
			{
				maxLon = record.lon;
			}
		}
	}

	m_pLayer->m_mbr.xmin = minLon;
	m_pLayer->m_mbr.xmax = maxLon;
	m_pLayer->m_mbr.ymin = minLat;
	m_pLayer->m_mbr.ymax = maxLat;

	return true;
}

void ShipMonitoringXMLFile::Draw(CDC* pDC, Scaler* scaler, double offset)
{
	POINT* p = new POINT[m_records.size()];

	CPen newPen(PS_SOLID, 1, RGB(94, 167, 63));
	CBrush newBrush(RGB(94, 167, 63));
	CPen* oldPen = pDC->SelectObject(&newPen);
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	for (size_t i = 0; i < m_records.size(); i++)
	{
		double x = m_records.at(i).lon;
		double y = m_records.at(i).lat;
		projection(x, y);
		scaler->WorldToDevice(x, y, &(p[i].x), &(p[i].y));

		pDC->Ellipse(p[i].x - 5, p[i].y - 5, p[i].x + 5, p[i].y + 5);

	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete[] p;
}