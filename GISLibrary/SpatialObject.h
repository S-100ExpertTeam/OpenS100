#pragma once

#include "Enum_S100_FileType.h"
#include "Namespace_GISLibrary.h"
#include "SpatialObjectType.h"

using namespace GISLibrary;

class Layer;
class Scaler;
class SpatialObject
{
public:
	SpatialObject(D2D1Resources* d2d1);
	virtual ~SpatialObject();

public:
	Layer* m_pLayer = nullptr;
	S100_FileType m_FileType = S100_FileType::FILE_NONE; 
	SpatialObjectType m_ObejctType = SpatialObjectType::SpatialObject;

private:
	CString m_FilePath; // Full path of file (including name)
	CString m_FileName; // File name (including extension)

protected:
	D2D1Resources* D2 = nullptr;

public:
	virtual bool Open(CString _filepath) = 0;
	virtual bool Save(std::wstring path) { return true; }

	virtual void Draw(CDC *pDC, Scaler *scaler, double offset = 0) {};
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0) {};
	virtual void Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset = 0) {};
	virtual void Draw(D2D1Resources* D2, Scaler* scaler) {};
	virtual void Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};
	virtual void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};

	virtual int getEPSGNumber() { return 0; }

	void SetLayer(Layer* value);
	Layer* GetLayer();

	void SetFilePath(CString value);
	CString GetFilePath();

	void SetFileName(CString value);
	CString GetFileName();

	void SetFileType(S100_FileType value);
	S100_FileType GetFileType();

	SpatialObjectType getSpatialObjectType();
};