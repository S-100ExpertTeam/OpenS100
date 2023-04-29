#include "stdafx.h"
#include "SpatialObject.h"

SpatialObject::SpatialObject(void)
{

}

SpatialObject::~SpatialObject(void)
{

}

void SpatialObject::SetLayer(Layer* value)
{
	m_pLayer = value;
}

Layer* SpatialObject::GetLayer()
{
	return m_pLayer;
}

void SpatialObject::SetFilePath(CString& value)
{
	m_FilePath = value;
	int index= value.ReverseFind('\\');
	auto name= value.Mid(index + 1);
	//Includes file name and extension.
	SetFileName(name);
}

CString SpatialObject::GetFilePath()
{
	return m_FilePath;
}

void SpatialObject::SetFileName(CString& value)
{
	m_FileName = value;
}


CString SpatialObject::GetFileName()
{
	return m_FileName;
}

void SpatialObject::SetFileType(S100_FileType value)
{
	m_FileType = value;
}

S100_FileType SpatialObject::GetFileType()
{
	return m_FileType;
}