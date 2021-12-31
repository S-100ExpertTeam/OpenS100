#include "stdafx.h"
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

bool BackgroundSHPFile::Open(CString path)
{
	int indexOfSplit = path.ReverseFind('\\');

	CFile fdSHP, fdSHX;
	CFileException e;

	// Open the SHP file.
	if (!fdSHP.Open(path, CFile::modeRead, &e))
	{
		OutputDebugString(_T("SHP File Open Error!"));
		return false;
	}

	//CString with path of SHX file.
	CString ShxFilePath = path.Left(path.GetLength() - 3) + _T("shx");

	// Open the SHX file.
	if (!fdSHX.Open(ShxFilePath, CFile::modeRead, &e))
	{
		OutputDebugString(_T("SHX File Open Error!"));
		return false;
	}	

	//Find out the number of records.
	int tmp;

	fdSHX.Seek(24, CFile::begin);
	fdSHX.Read(&tmp, 4);
	SwapWord(4, &tmp);

	int SHXFileSize = tmp * 2;
	recordCount = (SHXFileSize - 100) / 8;

	// Get shape type
	fdSHP.Seek(32, CFile::begin);

	int shapeType = 0;
	fdSHP.Read(&shapeType, sizeof(int));

	if (shapeType != 5)
	{
		OutputDebugString(_T("Invalied shape type\n"));
		return false;
	}

	fdSHP.Seek(100, CFile::begin);
	fdSHP.Read(&shapeType, sizeof(int));

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