#include "stdafx.h"
#include "Layer.h"
#include "S100Utilities.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"

Layer::Layer(void)
{
	On = true;
	Info = false;
}

Layer::~Layer(void)
{
	if (m_spatialObject)
	{
		delete m_spatialObject;
		m_spatialObject = nullptr;
	}
}

bool Layer::Open(CString _filepath)
{
	auto extension = LibMFCUtil::GetExtension(_filepath);

	if (nullptr == m_spatialObject)
	{
		m_spatialObject = new SHPFile();
	}

	m_spatialObject->m_pLayer = this;

	CString strFolderPath;
	if (_filepath.Find(TEXT(":\\")) > 0)
	{
		strFolderPath = _filepath;
	}
	else
	{
		::GetModuleFileName(nullptr, strFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
		strFolderPath.ReleaseBuffer();
		if (strFolderPath.Find('\\') != -1)
		{
			for (int i = strFolderPath.GetLength() - 1; i >= 0; i--)
			{
				TCHAR ch = strFolderPath[i];
				strFolderPath.Delete(i);
				if (ch == '\\') break;
			}
		}
		strFolderPath.Append(TEXT("\\") + _filepath);
	}
	return m_spatialObject->Open(strFolderPath);
}

void Layer::Draw(CDC *pDC, Scaler *scaler, double offset)
{
	// only draw layers when it's on.
	if (On == true)
	{
		m_spatialObject->Draw(pDC, scaler, offset);
	}
}

void Layer::Draw(HDC &hDC, Scaler *scaler, double offset)
{
	// only draw layers when it's on.
	if (On && m_spatialObject)
	{
		m_spatialObject->Draw(hDC, scaler, offset);
	}
}

void Layer::Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset)
{
	// only draw layers when it's on.
	if (On)
	{
		m_spatialObject->Draw(hDC, scaler, priority, geoType, offset);
	}
}

void Layer::Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX, double offsetY)
{
	// only draw layers when it's on.
	if (On == TRUE)
	{
		m_spatialObject->Draw(pRenderTarget, scaler, offsetX, offsetY);
	}
}

void Layer::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX, double offsetY)
{
	// only draw layers when it's on.
	if (On == TRUE)
	{
		m_spatialObject->Draw(pRenderTarget, pDXFactory, scaler, offsetX, offsetY);
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

CString Layer::GetLayerName()
{
	return m_spatialObject->GetFileName();
}

CString Layer::GetLayerPath()
{
	return m_spatialObject->GetFilePath();
}

CString Layer::GetLayerType()
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