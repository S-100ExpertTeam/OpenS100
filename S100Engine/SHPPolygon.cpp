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

void SHPPolygon::Read(int index, std::istream& fdSHP, std::istream& fdSHX)
{
	int offset = 0;

	fdSHX.seekg(100 + (index * 8));
	fdSHX.read(reinterpret_cast<char*>(&offset), 4);
	SwapWord(4, &offset);
	offset *= 2;

	fdSHP.seekg(offset + 12);

	// Read the record and save it.
	// MBR 
	double box[4] = { 0, 0, 0, 0 };
	
	fdSHP.read(reinterpret_cast<char*>(box), sizeof(double) * 4);

	// The number of parts.
	fdSHP.read(reinterpret_cast<char*>(&numParts), sizeof(int));

	// The number of points.
	numPoints = 0;
	fdSHP.read(reinterpret_cast<char*>(&numPoints), sizeof(int));

	// Arrangement of parts.
	parts = new int[numParts];
	memset(parts, 0, sizeof(int) * numParts);

	for (int i = 0; i < numParts; i++) 
	{
		fdSHP.read(reinterpret_cast<char*>(&parts[i]), sizeof(int));
	}

	// Point arrangement.
	points = new SHPPoint[numPoints];

	for (int j = 0; j < numPoints; j++) {
		fdSHP.read(reinterpret_cast<char*>(&points[j].x), sizeof(double));
		fdSHP.read(reinterpret_cast<char*>(&points[j].y), sizeof(double));
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