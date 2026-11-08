#pragma once
#include <string>

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
	std::wstring m_FilePath; // Full path of file (including name)
	std::wstring m_FileName; // File name (including extension), or the stem for HDF5 datasets


public:
	virtual bool Open(const std::wstring& _filepath) = 0;
	virtual bool OpenMetadata(const std::wstring& _filepath) = 0;
	virtual bool Save(std::wstring path) { return true; }

	// Legacy GDI drawing entry used by shapefile layers and the exchange-set coverage view.
	// S-100 datasets do not draw themselves; renderers do (Phase 1 step 2).
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0) {};

	virtual int getEPSGNumber() { return 0; }

	void SetLayer(Layer* value);
	Layer* GetLayer();

	void SetFilePath(const std::wstring& value);
	std::wstring GetFilePath() const;

	void SetFileName(const std::wstring& value);
	std::wstring GetFileName() const;

	void SetFileType(S100_FileType value);
	S100_FileType GetFileType();

	SpatialObjectType getSpatialObjectType();
};