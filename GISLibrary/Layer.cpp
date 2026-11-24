#include "stdafx.h"
#include "Layer.h"

Layer::Layer(void)
{
	On = true;
	Info = false;
	m_spatialObject = nullptr;
}

Layer::~Layer(void)
{
	if (m_spatialObject)
	{
		delete m_spatialObject;
		m_spatialObject = nullptr;
	}
}

void Layer::Draw(GISLibrary::ViewFrame& frame, Scaler *scaler, double offset)
{
	// only draw layers when it's on.
	if (On && m_spatialObject)
	{
		m_spatialObject->Draw(frame, scaler, offset);
	}
}

bool Layer::IsOn()
{
	return On;
}

bool Layer::IsOn(int scale)
{
	auto min = GetScaleMinimum();
	auto max = GetScaleMaximum();
	if (0 == max ||
		0 == min ||
		min < max)
	{
		return true;
	}
	else if (min >= scale &&
		max <= scale)
	{
		return true;
	}

	return false;
}

bool Layer::InfoOn()
{
	return Info;
}

void Layer::LayerOn()
{
	On = true;
}


void Layer::LayerOff()
{
	On = false;
}

void Layer::LayerInfoOn()
{
	Info = true;
}

void Layer::LayerInfoOff()
{
	Info = false;
}

std::wstring Layer::GetLayerName()
{
	return m_spatialObject->GetFileName();
}

std::wstring Layer::GetLayerPath()
{
	return m_spatialObject->GetFilePath();
}

std::wstring Layer::GetLayerType()
{
	if (m_spatialObject->m_FileType == S100_FileType::FILE_Shape)
	{
		return L"FILE_Shape";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_57)
	{
		return L"FILE_S_57";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR)
	{
		return L"FILE_S_100_VECTOR";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		return L"FILE_S_100_GRID_H5";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_BAG)
	{
		return L"FILE_S_100_GRID_BAG";
	}
	return L"";
}


void Layer::SetMBR(MBR& value)
{
	m_mbr.SetMBR(value);
}

MBR Layer::GetMBR()
{
	return m_mbr;
}

MBR* Layer::GetMBRPointer()
{
	return &m_mbr;
}

void Layer::SetSpatialObject(SpatialObject* value)
{
	m_spatialObject = value;
}

SpatialObject* Layer::GetSpatialObject()
{
	return m_spatialObject;
}

bool Layer::IsS100Layer()
{
	if (nullptr != GetSpatialObject())
	{
		auto type = GetSpatialObject()->GetFileType();

		if (type == S100_FileType::FILE_S_100_VECTOR ||
			type == S100_FileType::FILE_S_100_GRID_BAG ||
			type == S100_FileType::FILE_S_100_GRID_H5
			)
		{
			return true;
		}
	}

	return false;
}

S100_FileType Layer::GetFileType()
{
	if (nullptr != m_spatialObject)
	{
		return m_spatialObject->m_FileType;
	}

	return S100_FileType::FILE_NONE;
}

int Layer::GetScaleMinimum()
{
	return scaleMinimum;
}

void Layer::SetScaleMiminum(int value)
{
	if (value >= 0)
	{
		scaleMinimum = value;
	}
	else
	{
		scaleMinimum = 0;
	}
}

int Layer::GetScaleMaximum()
{
	return scaleMaximum;
}

void Layer::SetScaleMaximum(int value)
{
	if (value >= 0)
	{
		scaleMaximum = value;
	}
	else
	{
		scaleMaximum = 0;
	}
}

void Layer::SetID(int value)
{
	id = value;
}

int Layer::GetID()
{
	return id;
}
