#include "stdafx.h"
#include "DRDirectoryInfo.h"
#include "ISO8211Fuc.h"

DRDirectoryInfo::DRDirectoryInfo()
{

}

DRDirectoryInfo::DRDirectoryInfo(int directoryCount)
{
	m_count = directoryCount;
	directories = new DRDirectory[m_count];
}

DRDirectoryInfo::~DRDirectoryInfo()
{	
	Delete();
}

bool DRDirectoryInfo::ReadDir(DRReader drReader, BYTE*& buf)
{
	for (int i = 0; i < m_count; i++)
	{
		directories[i].tag[0] = (char)buf[0];
		buf++;
		directories[i].tag[1] = (char)buf[0];
		buf++;
		directories[i].tag[2] = (char)buf[0];
		buf++;
		directories[i].tag[3] = (char)buf[0];
		buf++;
		directories[i].length = atoi(buf, drReader.m_fieldLength);
		directories[i].pos = atoi(buf, drReader.m_fieldPosition);
	}
	return true;
}

void DRDirectoryInfo::ReAllocateDirectory(int count)
{
	Delete();
	m_count = count;
	directories = new DRDirectory[m_count];
}

DRDirectory* DRDirectoryInfo::GetDirectory(int count)
{
	return &(directories[count]);
}

void DRDirectoryInfo::Delete()
{
	if (nullptr != directories)
	{
		delete[] directories;
		directories = nullptr;
	}
}