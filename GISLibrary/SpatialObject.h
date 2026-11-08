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
	SpatialObject();
	virtual ~SpatialObject();

public:
	Layer* m_pLayer = nullptr;
	S100_FileType m_FileType = S100_FileType::FILE_NONE; 
	SpatialObjectType m_ObejctType = SpatialObjectType::SpatialObject;

private:
	CString m_FilePath; // Full path of file (including name)
	CString m_FileName; // File name (including extension)


public:
	virtual bool Open(CString _filepath) = 0;
	virtual bool OpenMetadata(CString _filepath) = 0;
	virtual bool Save(std::wstring path) { return true; }

	// Legacy GDI drawing entry used by shapefile layers and the exchange-set coverage view.
	// S-100 datasets do not draw themselves; renderers do (Phase 1 step 2).
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0) {};

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