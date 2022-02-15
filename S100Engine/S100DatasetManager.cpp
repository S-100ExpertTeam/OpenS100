#include "stdafx.h"
#include "S100DatasetManager.h"
#include "S100PCManager.h"

S100DatasetManager::S100DatasetManager()
{

}

S100DatasetManager::~S100DatasetManager()
{
	for (auto i = s101Dataset.begin(); i != s101Dataset.end(); i++)
	{
		delete *i;
	}
	s101Dataset.clear();
}


bool S100DatasetManager::OpenS101(CString _path)
{
	S101Dataset* s101Dataset = new S101Dataset();
	bool ret = s101Dataset->OpenS101(_path);

	if (!ret)
	{
		OutputDebugString(_T("Failed to call LoadBaseCell\n"));
		return false;
	}
	this->s101Dataset.push_back(s101Dataset);
	GeneratePathDatasetMap();
	return true;
}

bool S100DatasetManager::OpenS101SENC(CString _path)
{
	S101Dataset* s101Dataset = new S101Dataset();
	bool ret = s101Dataset->OpenS101SENC(_path);

	if (!ret)
	{
		OutputDebugString(_T("Failed to open S101 SENC\n"));
		return false;
	}

	this->s101Dataset.push_back(s101Dataset);
	GeneratePathDatasetMap();
	return true;
}

void S100DatasetManager::GeneratePathDatasetMap()
{
	mapDataset.clear();
	for (auto i = s101Dataset.begin(); i != s101Dataset.end(); i++)
	{
		mapDataset.insert({ (*i)->GetFilePath(), *i });
	}
}


S101Dataset* S100DatasetManager::GetS101Dataset(CString path)
{
	auto i = mapDataset.find(path);
	if (i != mapDataset.end())
	{
		return i->second;
	}
	return nullptr;
}


void S100DatasetManager::CreateS100DrawingUnit(ID2D1Factory1* pDirect2dFactory, S100PCManager& s100PCManager)
{
	for (auto i = s101Dataset.begin(); i != s101Dataset.end(); i++)
	{
		(*i)->CreateS100DrawingUnit(pDirect2dFactory, s100PCManager);
	}
}


void S100DatasetManager::DeleteS100DrawingUnit()
{
	for (auto i = s101Dataset.begin(); i != s101Dataset.end(); i++)
	{
		(*i)->DeleteS100DrawingUnit();
	}
}


void S100DatasetManager::GetBoundingBox(FLOAT& xmin, FLOAT& ymin, FLOAT& xmax, FLOAT& ymax)
{
	for (auto i = s101Dataset.begin(); i != s101Dataset.end(); i++)
	{
		if (i == s101Dataset.begin())
		{
			xmin = (*i)->xmin;
			ymin = (*i)->ymin;
			xmax = (*i)->xmax;
			ymax = (*i)->ymax;
		}
		else
		{
			if (xmin > (*i)->xmin) {
				xmin = (*i)->xmin;
			} else if (xmax < (*i)->xmax) {
				xmax = (*i)->xmax;
			}

			if (ymin > (*i)->ymin) {
				ymin = (*i)->ymin;
			} else if (ymax < (*i)->ymax) {
				ymax = (*i)->ymax;
			}
		}
	}
}


int S100DatasetManager::GetS101DatasetCount()
{
	return (int)s101Dataset.size();
}


S101Dataset* S100DatasetManager::Get101Dataset(int index)
{
	return s101Dataset.at(index);
}