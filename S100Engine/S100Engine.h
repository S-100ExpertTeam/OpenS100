#pragma once

#ifndef __AFXWIN_H__
	#error "Include 'stdafx.h' before including this file for PCH."
#endif

#include "resource.h"		
#include "S100PCManager.h"
#include "S100Render.h"

#include <vector>

class S100Engine
{
public:
	S100Engine();
	virtual ~S100Engine();

public:
	ID2D1Factory1* pDirect2dFactory = nullptr;
	S100PCManager      s100PCManager;
	S100Render         s100Render;

	std::vector<S100DrawingUnit*> s100DrawingUnit;

private:
	float zoom;
	float x;
	float y;

public:
	bool OpenS100PortrayalCatalogue(CString folderPath);
	void Draw(ID2D1Factory1* pDirect2dFactory, ID2D1DCRenderTarget* pRenderTarget, IDWriteTextFormat* pTextFormat, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyleS101Solid, ID2D1StrokeStyle1* pStrokeStyleS101Dash, CRect& rectView);
	void DrawSymbol(std::wstring _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, D2D1_POINT_2F point, FLOAT rotation = 0, FLOAT scale = 5);
	//bool OpenS100ColorProfile(std::wstring _path);
	bool OpenS100Symbol(std::wstring _path);
	bool OpenS100AreaFills(std::wstring _path);
	bool OpenS100LineStyles(std::wstring _path);
	void CreateSVGGeometry(ID2D1Factory1* pDirect2dFactory);
	void CreateBitmapImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid);
	void CreateLineImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid);
	void CreateBackgroundGeometry(ID2D1Factory1* pD2Factory);
	void CreateBitmapBrush(ID2D1RenderTarget* pRenderTarget);
	void DeleteBitmapBrush();
	void DeleteBitmapImage();
	void SetD2Factory(ID2D1Factory1* factory);
};

class CS100EngineApp : public CWinApp
{
public:
	CS100EngineApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
