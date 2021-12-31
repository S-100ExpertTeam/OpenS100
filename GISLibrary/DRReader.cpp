#include "StdAfx.h"
#include "DRReader.h"
#include "ISO8211Fuc.h"

DRReader::DRReader()
{
}

DRReader::~DRReader()
{
}

bool DRReader::ReadReader(BYTE*& buf)
{
	m_recordLength = atoi(buf, 5);
	buf+= 7;
	m_fieldAreaLoc = atoi(buf, 5);
	buf+= 3;
	m_fieldLength = atoi(buf, 1);
	m_fieldPosition = atoi(buf, 1);
	buf += 2;
	return true;
}