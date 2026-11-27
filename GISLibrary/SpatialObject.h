#pragma once
#include <string>

#include "DeviceTypes.h"
#include "Enum_S100_FileType.h"
#include "SpatialObjectType.h"
#include "ViewFrame.h"

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
	virtual bool Save(std::wstring /*path*/) { return true; }

	// Draws on the frame of a view: shapefile layers, an exchange catalogue and the System
	// Database chart draw themselves. The instructions of an S-100 dataset are drawn by the
	// layer manager.
	virtual void Draw(GISLibrary::ViewFrame& /*frame*/, Scaler* /*scaler*/, double /*offset*/ = 0) {};

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