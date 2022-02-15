#include "StdAfx.h"
#include "SHPFile.h"

SHPFile::SHPFile(void)
{

}

SHPFile::~SHPFile(void)
{
	for (unsigned int i = 0; i < m_nRecords; i++) {
		delete m_pSHPObject[i];
	}
	delete[] m_pSHPObject;
}

// Read the file and store the data in the layer.
bool SHPFile::Open(CString _filepath)
{
	SetFilePath(_filepath);

	m_FileType = FILE_Shape;

	CFile fdSHP, fdSHX;
	CFileException e;

	// Open the SHP file.
	if (!fdSHP.Open(_filepath, CFile::modeRead, &e))
	{
		OutputDebugString(_T("SHP File Open Error!"));
		return FALSE;
	}

	// CString with path of SHX file.
	CString ShxFilePath = _T("");
	// Copy without extension.
	for (int i = 0; i < (_filepath.GetLength() - 3); i++)
	{
		ShxFilePath.AppendChar(_filepath.GetAt(i));
	}

	// Attach extensions to the path of SHX file.
	ShxFilePath.AppendFormat(_T("shx"));

	// Open the SHX file.
	if (!fdSHX.Open(ShxFilePath, CFile::modeRead, &e))
	{
		OutputDebugString(_T("SHX File Open Error!"));
		return FALSE;
	}

	// Find out the number of records.
	int tmp;

	fdSHX.Seek(24, CFile::begin);
	fdSHX.Read(&tmp, 4);
	SwapWord(4, &tmp);

	int SHXFileSize = tmp * 2;
	m_nRecords = (SHXFileSize - 100) / 8;
	
	// Get shape type
	fdSHP.Seek(32, CFile::begin);
	fdSHP.Read(&m_nShapeType, sizeof(int));

	if (!((m_nShapeType == GEOPOINT) || (m_nShapeType == GEOPOLYLINE) || (m_nShapeType == GEOPOLYGON))) {
		OutputDebugString(_T("Invalid shape type.\n"));
		return FALSE;
	}

	// Get a MBR
	double tmpMBR;
	fdSHP.Read(&tmpMBR, sizeof(double));
	m_pLayer->m_mbr.xmin = tmpMBR;
	fdSHP.Read(&tmpMBR, sizeof(double));
	m_pLayer->m_mbr.ymin = tmpMBR;
	fdSHP.Read(&tmpMBR, sizeof(double));
	m_pLayer->m_mbr.xmax = tmpMBR;
	fdSHP.Read(&tmpMBR, sizeof(double));
	m_pLayer->m_mbr.ymax = tmpMBR;

	projection(m_pLayer->m_mbr.xmin, m_pLayer->m_mbr.ymin);
	projection(m_pLayer->m_mbr.xmax, m_pLayer->m_mbr.ymax);

	// Get shape data
	switch (m_nShapeType) 
	{
		case GEOPOINT:
		{
			m_pSHPObject = new Geometry*[m_nRecords];

			for (unsigned int i = 0; i < m_nRecords; i++) {
				// Finding the location of the record in SHX.
				int offset;

				fdSHX.Seek(100 + (i * 8), CFile::begin);
				fdSHX.Read(&offset, 4);
				SwapWord(4, &offset);
				offset *= 2;

				// Read the record and save it.
				double x, y;

				fdSHP.Seek(offset + 12, CFile::begin);
				fdSHP.Read(&x, 8);
				fdSHP.Read(&y, 8);

				projection(x, y);

				m_pSHPObject[i] = new GeoPoint(x, y);
			}

			break;
		}
		case GEOPOLYLINE:
		{
			m_pSHPObject = new Geometry*[m_nRecords];

			for (unsigned int i = 0; i < m_nRecords; i++) {
				m_pSHPObject[i] = new GeoPolyline();

				// Finding the location of the record in SHX.
				int offset;

				fdSHX.Seek(100 + (i * 8), CFile::begin);
				fdSHX.Read(&offset, 4);
				SwapWord(4, &offset);
				offset *= 2;
				
				// Read the record and save it.
				// MBR 
				MBR polylineMBR;
				fdSHP.Seek(offset + 12, CFile::begin);

				fdSHP.Read(&tmpMBR, sizeof(double));
				polylineMBR.xmin = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polylineMBR.ymin = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polylineMBR.xmax = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polylineMBR.ymax = tmpMBR;
			
				projection(polylineMBR.xmin, polylineMBR.ymin);
				projection(polylineMBR.xmax, polylineMBR.ymax);

				// number of parts.
				int numParts;
				fdSHP.Read(&numParts, sizeof(int));
				
				// number of points.
				int numPoints;
				fdSHP.Read(&numPoints, sizeof(int));

				// array of the part
				int *parts = new int[numParts];

				for (int j = 0; j < numParts; j++) {
					fdSHP.Read(&parts[j], sizeof(int));
				}

				// array of the points
				GeoPoint *points = new GeoPoint[numPoints];

				for (int j = 0; j < numPoints; j++) {
					double x, y;

					fdSHP.Read(&x, sizeof(double));
					fdSHP.Read(&y, sizeof(double));
					
					projection(x, y);

					points[j].SetPoint(x, y);
				}

				((GeoPolyline *)m_pSHPObject[i])->m_mbr = polylineMBR;
				((GeoPolyline *)m_pSHPObject[i])->NumParts = numParts;
				((GeoPolyline *)m_pSHPObject[i])->NumPoints = numPoints;
				((GeoPolyline *)m_pSHPObject[i])->Parts = parts;
				((GeoPolyline *)m_pSHPObject[i])->Points = points;
				((GeoPolyline *)m_pSHPObject[i])->CPoints = new CPoint[numPoints];
			}

			break;
		}
		case GEOPOLYGON:
		{
			m_pSHPObject = new Geometry*[m_nRecords];

			for (unsigned int i = 0; i < m_nRecords; i++) {
				m_pSHPObject[i] = new GeoPolyline();

				// Finding the location of the record in SHX.
				int offset;

				fdSHX.Seek(100 + (i * 8), CFile::begin);
				fdSHX.Read(&offset, 4);
				SwapWord(4, &offset);
				offset *= 2;
				
				// read recoad then save
				// MBR 
				MBR polygonMBR;
				fdSHP.Seek(offset + 12, CFile::begin);

				fdSHP.Read(&tmpMBR, sizeof(double));
				polygonMBR.xmin = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polygonMBR.ymin = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polygonMBR.xmax = tmpMBR;
				fdSHP.Read(&tmpMBR, sizeof(double));
				polygonMBR.ymax = tmpMBR;
				
				projection(polygonMBR.xmin, polygonMBR.ymin);
				projection(polygonMBR.xmax, polygonMBR.ymax);

				// num of the parts
				int numParts;
				fdSHP.Read(&numParts, sizeof(int));
				
				// num of the points
				int numPoints;
				fdSHP.Read(&numPoints, sizeof(int));

				// array of the parts
				int *parts = new int[numParts];

				for (int j = 0; j < numParts; j++) {
					fdSHP.Read(&parts[j], sizeof(int));
				}

				// array of the points
				GeoPoint *points = new GeoPoint[numPoints];

				for (int j = 0; j < numPoints; j++) {
					double x, y;

					fdSHP.Read(&x, sizeof(double));
					fdSHP.Read(&y, sizeof(double));

					projection(x, y);

					points[j].SetPoint(x, y);
				}

				((GeoPolyline *)m_pSHPObject[i])->m_mbr = polygonMBR;
				((GeoPolyline *)m_pSHPObject[i])->NumParts = numParts;
				((GeoPolyline *)m_pSHPObject[i])->NumPoints = numPoints;
				((GeoPolyline *)m_pSHPObject[i])->Parts = parts;
				((GeoPolyline *)m_pSHPObject[i])->Points = points;
				((GeoPolyline *)m_pSHPObject[i])->CPoints = new CPoint[numPoints];
			}

			break;
		}
	}
	

	return TRUE;
}

void SHPFile::Draw(HDC &hDC, Scaler *scaler, double offset)
{
	double zero  = 0;
	projection(offset, zero);

	for (unsigned int i = 0; i < m_nRecords; i++) {
		m_pSHPObject[i]->DrawGeometry(hDC, scaler, offset);
	}
}

//transforms Big Endian and Little Endian into each other.
void SHPFile::SwapWord(int _length, void *_pData)
{
	//  odd  _length : You don't have to change the median value of "1".
	//  even  _length : A small value of the index of the median two numbers.
	int center_index = _length / 2;
	unsigned char temp;

	// Byte Ordering Conversion
	for (int i = 0; i < center_index; i++) {
		temp = ((unsigned char *)_pData)[i];
		((unsigned char *)_pData)[i] = ((unsigned char *)_pData)[_length - i - 1];
		((unsigned char *)_pData)[_length - i - 1] = temp;
	}
}