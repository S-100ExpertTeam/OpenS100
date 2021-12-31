#pragma once

#include "LayerManager.h"
#include "D2D1Resources.h"
#include "D2D1ChartResources.h"
#include "BasicFile.h"

#include "../S100Engine/S100Engine.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/ENCGeometry.h"
#include "..\\extlibs\\Clipper\\include\\clipper.hpp"


#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

struct ID2D1Factory1;
struct ID2D1StrokeStyle1;
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
	GISLibrary::D2D1ChartResources D2ChartResources;
	std::vector<BasicFile> BasicFilePath;
	bool checkRouteDetection = false;

public:
	Scaler* GetScaler();
	LayerManager* GetLayerManager();
	int GetLayerIndex(CString pathName);
	void InitS100Engine();
	void InitLibrary();
	bool AddBackgroundLayer(CString _filepath);
	bool AddLayer(CString _filepath);
	void Draw(HDC &hDC, int offset = 0);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, double offsetX = 0, double offsetY = 0);
	void BuildPortrayalCatalogue(Layer* l);
	void DeletePortrayalCatalogue(Layer* l);
	void S101RebuildPortrayal();
	void DrawValidationLayers(HDC &hDC, int offset = 0);
	void DrawOverlay(HDC &hDC, int type, int offset = 0);
	void DrawShipMonitoringLayer(HDC &hDC, int offset = 0);
	void GetLayer(int index, Layer *_layer);
	Layer* GetLayer(int index);

	void DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale = 5);
	void DrawScaleBar();

	CString GetLayerName(int index);
	BOOL IsOn(int index);
	void DeleteLayer(int index);
	void DeleteLayer(CString filepath);
	void DeleteAllLayer();
	void ReMBR();

	void ClearInformationLayer(int index);
	void ClearInformationLayer(CString filepath);
	void ClearAllInformationLayer();
	void SaveLayer(CString filename, CString extend, int index);
	void SaveLayer(CString filename, CString extend, Layer* pLayer);

	void SetViewMBR(RECT r);

	/*
	**  Get number of saved layers 
	*/
	int GetLayerCount();
	std::vector<Layer*> GetAllLayer();

	MBR* GetMBR();
						
	/////////////////////////////////////////////////////////////////////////////
	//  Scalar Method
	//  Coordinate change.
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice(D2D1_POINT_2F& p);
	void DeviceToWorld(D2D1_POINT_2F& p);

	// calc distance
	double GetDistanceScreen(int x1, int y1, int x2, int y2);

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

	// rotate screen
	void Rotate(LONG *sx, LONG *sy, double degree);
	void RotateMap(double degree);	// Maintain the range of rotateDegrees to (0, 360).
	void NorthUp();


	bool PtInMap(double _x, double _y);

	void ChangeDisplayFont();

	void SetDrawBackground(bool on);
	bool GetDrawBackground();

	void SetExPCPath(CString value);
	CString GetExPCPath();

	void SetExFCPath(CString value);
	CString GetExFCPath();

	// Initial FC, PC designation to load.


	// FC, PC loading
	void BasicFileSetting();

	std::wstring GetColorTable();


	////////////////////////////////////////////// 
	// For S-100 ECDIS(NAVIK) co-work DLL
	// function that needs to be implemented additionally.
	// inMx, inMy, outMx, outMy - Projected coordinates.
	// 
	// information related Own Ship
	void SetOwnShipPosition(double lon, double lat);
	void SetOwnShipSize(double length, double width);

	//// Route
	ID2D1PathGeometry* combine_twice_path_geometries(ID2D1Factory1*& srcfactory, ID2D1PathGeometry* geo1, ID2D1PathGeometry* geo2);
	double GetAngleFromLegs(double _heading1, double _heading2, bool _radian);
	bool IsRight(double _x1Leg1, double _y1Leg1, double _x2Leg1, double _y2Leg1,
		double _x1Leg2, double _y1Leg2, double _x2Leg2, double _y2Leg2);
};

extern CGISLibraryApp* gisLib;