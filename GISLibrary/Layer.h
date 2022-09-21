#pragma once
#include "Namespace_GISLibrary.h"
#include "Enum_S100_FileType.h"

#include "..\\GeoMetryLibrary\\MBR.h"

class SpatialObject;
class Scaler;
//Class to save information for layers.
class Layer
{

public:
	Layer();
	virtual ~Layer();

public:
	int id = 0;

	bool On;
	bool Info;

	// area info
	MBR			  m_mbr;
	SpatialObject *m_spatialObject = nullptr;
	int scaleMinimum = 0;
	int scaleMaximum = 0;

public:
	virtual bool Open(CString _filepath);
	virtual void Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler) {};
	void Draw(CDC *pDC, Scaler *scaler, double offset = 0);
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	void Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset);
	void Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX = 0, double offsetY = 0);
	bool IsOn();
	bool IsOn(int scale);
	bool InfoOn();
	void LayerOn();  
	void LayerOff();
	void LayerInfoOn();
	void LayerInfoOff();

	CString GetLayerName();
	CString GetLayerPath();
	CString GetLayerType();

	void SetMBR(MBR& value);
	MBR GetMBR();
	MBR* GetMBRPointer();
	void SetSpatialObject(SpatialObject* value);
	SpatialObject* GetSpatialObject();
	bool IsS100Layer();

	S100_FileType GetFileType();

	int GetScaleMinimum();
	void SetScaleMiminum(int value);

	int GetScaleMaximum();
	void SetScaleMaximum(int value);

	void SetID(int value);
	int GetID();
};