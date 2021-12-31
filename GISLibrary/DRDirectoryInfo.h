#pragma once

#include "DRReader.h"
#include "DRDirectory.h"
#include <iostream>
#include <fstream>

class DRDirectoryInfo
{
public:
	DRDirectoryInfo();
	DRDirectoryInfo(int directoryCount);
	virtual ~DRDirectoryInfo();

public:
	DRDirectory* directories = nullptr;
	int m_count = 0;

public:
	bool ReadDir(DRReader drReader, BYTE*& buf);
	DRDirectory* GetDirectory(int count);
	void ReAllocateDirectory(int count);
	void Delete();
};