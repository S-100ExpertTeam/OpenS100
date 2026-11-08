#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include "BackgroundSHPFile.h"


BackgroundSHPFile::BackgroundSHPFile()
{

}

BackgroundSHPFile::~BackgroundSHPFile()
{
	for (auto i = vecPolygons.begin(); i != vecPolygons.end(); i++)
	{
		delete *i;
	}

	vecPolygons.clear();
}

int BackgroundSHPFile::GetRecordCount()
{
	return (int)vecPolygons.size();
}

SHPPolygon* BackgroundSHPFile::GetPolygon(unsigned index)
{
	if (index < 0 || index >= vecPolygons.size())
		return nullptr;

	return vecPolygons.at(index);
}

bool BackgroundSHPFile::Open(const std::wstring& path)
{
	std::ifstream fdSHP(std::filesystem::path(path), std::ios::binary);
	if (!fdSHP)
	{
		return false;
	}

	// SHX file: same name, extension swapped.
	const std::wstring shxPath = path.substr(0, path.size() - 3) + L"shx";
	std::ifstream fdSHX(std::filesystem::path(shxPath), std::ios::binary);
	if (!fdSHX)
	{
		return false;
	}

	// Record count from the SHX header (big-endian file length in 16-bit words).
	int tmp = 0;
	fdSHX.seekg(24);
	fdSHX.read(reinterpret_cast<char*>(&tmp), 4);
	SwapWord(4, &tmp);
	const int SHXFileSize = tmp * 2;
	recordCount = (SHXFileSize - 100) / 8;

	// Shape type must be polygon (5).
	int shapeType = 0;
	fdSHP.seekg(32);
	fdSHP.read(reinterpret_cast<char*>(&shapeType), sizeof(int));
	if (shapeType != 5)
	{
		return false;
	}

	fdSHP.seekg(100);
	fdSHP.read(reinterpret_cast<char*>(&shapeType), sizeof(int));

	for (unsigned int i = 0; i < recordCount; i++)
	{
		SHPPolygon* polygon = new SHPPolygon;
		vecPolygons.push_back(polygon);
		polygon->Read(i, fdSHP, fdSHX);
	}

	return true;
}

void BackgroundSHPFile::SwapWord(int _length, void *_pData)
{
	// When _length is an odd number: "Central index -1", you don't need to change the median number.
	// When _length is an even number: a small value of the index of the positive center number.
	int center_index = _length / 2;
	unsigned char temp;

	// Byte Ordering change
	for (int i = 0; i < center_index; i++) {
		temp = ((unsigned char *)_pData)[i];
		((unsigned char *)_pData)[i] = ((unsigned char *)_pData)[_length - i - 1];
		((unsigned char *)_pData)[_length - i - 1] = temp;
	}
}