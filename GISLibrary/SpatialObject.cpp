#include "stdafx.h"
#include "PathUtil.h"
#include "SpatialObject.h"

SpatialObject::SpatialObject()
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

void SpatialObject::SetFilePath(const std::wstring& value)
{
	m_FilePath = value;
	SetFileName(s100::path::FileName(value)); // name with extension
}

std::wstring SpatialObject::GetFilePath() const
{
	return m_FilePath;
}

void SpatialObject::SetFileName(const std::wstring& value)
{
	m_FileName = value;
}

std::wstring SpatialObject::GetFileName() const
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

SpatialObjectType SpatialObject::getSpatialObjectType()
{
	return m_ObejctType;
}
