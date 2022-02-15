#pragma once
#include "S101Dataset.h"
#include "Class_S101_Base.h"

#include <map>
#include <vector>

class S100DatasetManager
{
public:
	S100DatasetManager();
	virtual ~S100DatasetManager();

public:
	FLOAT xmin = 0;
	FLOAT ymin = 0;
	FLOAT xmax = 0;
	FLOAT ymax = 0;

	std::vector<S101Dataset*> s101Dataset;
	std::map<CString, S101Dataset*> mapDataset;

public:
	bool OpenS101(CString _path);
	bool OpenS101SENC(CString _path);
	void GeneratePathDatasetMap();
	S101Dataset* GetS101Dataset(CString path);
	void CreateS100DrawingUnit(ID2D1Factory1* pDirect2dFactory, S100PCManager& s100PCManager);
	void DeleteS100DrawingUnit();
	void GetBoundingBox(FLOAT& xmin, FLOAT& ymin, FLOAT& xmax, FLOAT& ymax);
	int GetS101DatasetCount();
	S101Dataset* Get101Dataset(int index);
};