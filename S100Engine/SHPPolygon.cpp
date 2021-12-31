#include "stdafx.h"
#include "SHPPolygon.h"

SHPPolygon::SHPPolygon()
{

}

SHPPolygon::~SHPPolygon()
{
	delete[] parts;
	parts = nullptr;

	delete[] points;
	points = nullptr;
}

void SHPPolygon::SwapWord(int _length, void *_pData)
{
	// When _length is an odd number: "Central index -1", you don't need to change the median number.
	// When _length is an even number: a small value of the index of the positive center number.
	int center_index = _length / 2;
	unsigned char temp;

	// Byte Ordering translate
	for (int i = 0; i < center_index; i++) {
		temp = ((unsigned char *)_pData)[i];
		((unsigned char *)_pData)[i] = ((unsigned char *)_pData)[_length - i - 1];
		((unsigned char *)_pData)[_length - i - 1] = temp;
	}
}

void SHPPolygon::Read(int index, CFile& fdSHP, CFile& fdSHX)
{
	int offset = 0;

	fdSHX.Seek(100 + (index * 8), CFile::begin);
	fdSHX.Read(&offset, 4);
	SwapWord(4, &offset);
	offset *= 2;

	fdSHP.Seek(offset + 12, CFile::begin);

	// Read the record and save it.
	// MBR 
	double box[4] = { 0, 0, 0, 0 };
	
	fdSHP.Read(box, sizeof(double) * 4);

	// The number of parts.
	fdSHP.Read(&numParts, sizeof(int));

	// The number of points.
	numPoints = 0;
	fdSHP.Read(&numPoints, sizeof(int));

	// Arrangement of parts.
	parts = new int[numParts];
	memset(parts, 0, sizeof(int) * numParts);

	for (int i = 0; i < numParts; i++) 
	{
		fdSHP.Read(&parts[i], sizeof(int));
	}

	// Point arrangement.
	points = new SHPPoint[numPoints];

	for (int j = 0; j < numPoints; j++) {
		fdSHP.Read(&points[j].x, sizeof(double));
		fdSHP.Read(&points[j].y, sizeof(double));
	}
}


int SHPPolygon::GetNumParts()
{
	return numParts;
}


int SHPPolygon::GetNumPoints()
{
	return numPoints;
}


int SHPPolygon::GetNumPointsPerPart(int index)
{
	if (index == numParts - 1)
	{
		return numPoints - parts[index];
	}
	else if (index >= 0 && index < (numParts - 1))
	{
		return parts[index + 1] - parts[index];
	}

	return 0;
}


float SHPPolygon::GetX(int index)
{
	return (float)points[index].x;
}


float SHPPolygon::GetY(int index)
{
	return (float)points[index].y;
}