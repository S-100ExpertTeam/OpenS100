#pragma once
#include "SHPPolygon.h"

#include <vector>

class BackgroundSHPFile
{
public:
	BackgroundSHPFile();
	virtual ~BackgroundSHPFile();

public:
	unsigned int recordCount = 0;
	std::vector<SHPPolygon*> vecPolygons;

public:
	bool Open(CString path);
	int GetRecordCount();
	SHPPolygon* GetPolygon(unsigned index);

private:
	void SwapWord(int _length, void *_pData);
};