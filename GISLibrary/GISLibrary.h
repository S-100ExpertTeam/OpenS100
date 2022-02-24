#pragma once

#include "LayerManager.h"
#include "D2D1Resources.h"

#include "../S100Engine/S100Engine.h"

#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/ENCGeometry.h"

#include "../extlibs/Clipper/include/clipper.hpp"

#ifndef __AFXWIN_H__
#endif

class S100Engine;

class CGISLibraryApp
{
public:
	CGISLibraryApp();
	virtual ~CGISLibraryApp();

public:
	Scaler* m_pScaler = new Scaler();
	LayerManager* m_pLayerManager = new LayerManager(m_pScaler);
	GISLibrary::D2D1Resources D2;
	bool checkRouteDetection = false;

	// FC & PC for S-101
	FeatureCatalogue* fc = nullptr;
	PortrayalCatalogue* pc = nullptr;

public:
	Scaler* GetScaler();
	LayerManager* GetLayerManager();
	//int GetLayerIndex(CString pathName);
	void InitS100Engine();
	void InitLibrary();
	bool AddBackgroundLayer(CString _filepath);
	bool AddLayer(CString _filepath);
	void Draw(HDC &hDC, int offset = 0);
	void BuildPortrayalCatalogue(Layer* l);
	void S101RebuildPortrayal();
	Layer* GetLayer();

	void DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale = 5);
	void DrawScaleBar();

	CString GetLayerName();
	BOOL IsOn();
	void DeleteLayer();
	void DeleteLayer(CString filepath);
	void ReMBR();

	void SetViewMBR(RECT r);

	MBR* GetMBR();
						
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice(D2D1_POINT_2F& p);
	void DeviceToWorld(D2D1_POINT_2F& p);

	// setting screen
	void SetScreen(RECT *rect);
	void SetScreen(const RECT& rect);
	int GetCenterXScreen();
	int GetCenterYScreen();

	// Map's information shown on the screen.
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);

	double GetMapWidth();
	double GetMapHeight();

	void GetMap(MBR *mbr);

	// Scale info
	int GetCurrentScale();
	void   SetScale(int scale);
	void   UpdateScale();

	/*
	** Zoom in & Zoom out.
	** factor : %
	** Example  : If you want to zoom in 20%, insert "20" into the factor.
	*/
	void ZoomIn(double value);
	void ZoomIn(double value, int x, int y);
	void ZoomOut(double value);
	void ZoomOut(double value, int x, int y);
	void AdjustScreenMap();

	// move screen
	// Use it to move as many pixels as you want (not completed)
	void MoveMap(int x, int y);

	// Use to position a desired point (mx, my) at a desired location (sy, sy)
	void MoveMap(int sx, int sy, double mx, double my);

	bool PtInMap(double _x, double _y);

	void ChangeDisplayFont();

	std::wstring GetColorTable();

	FeatureCatalogue* GetFC();
	PortrayalCatalogue* GetPC();
};

extern CGISLibraryApp* gisLib;