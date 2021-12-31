#pragma once

#include "AreaFillInfo.h"
#include "S100PCManager.h"
#include "S100DatasetManager.h"
#include "BackgroundSHPFile.h"
#include "BackgroundLayer.h"

class S100DrawingUnit;

class S100Render
{
public:
	S100Render();
	virtual ~S100Render();

public:
	FLOAT xmin = 0;
	FLOAT ymin = 0;
	FLOAT xmax = 0;
	FLOAT ymax = 0;

	D2D1::Matrix3x2F geomMatrix;
	D2D1::Matrix3x2F screenMatrix;

	FLOAT mapOffsetX = 0;
	FLOAT mapOffsetY = 0;
	int centerX = 0;
	int centerY = 0;

	int screenOffsetX = 0;
	int screenOffsetY = 0;

	FLOAT _x = 90;
	FLOAT _y = 10;
	FLOAT _rotation = 0;
	int scale = 1000;

	BackgroundSHPFile backgroundSHPFile;
	BackgroundLayer backgroundLayer;
	
public:
	void DrawBitmapOnLine(
		ID2D1RenderTarget *pRenderTarget,
		D2D1_POINT_2F point1,
		D2D1_POINT_2F point2,
		ID2D1Bitmap *pBitmap,
		D2D1_COLOR_F penColor = D2D1::ColorF(D2D1::ColorF::Black),
		ID2D1StrokeStyle1 *stroke = nullptr);

	void DrawBitmapOnPolyline(
		ID2D1RenderTarget *pRenderTarget,
		D2D1_POINT_2F *points,
		int cnt,
		ID2D1Bitmap *pBitmap,
		D2D1_RECT_F viewRect, 
		D2D1_COLOR_F penColor = D2D1::ColorF(D2D1::ColorF::Black),
		ID2D1StrokeStyle1 *stroke = nullptr);

	bool InterSectLine(
		FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
		FLOAT x3, FLOAT y3, FLOAT x4, FLOAT y4);

	bool InterSectLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect);
	bool OverlapLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect);
	bool PointInRect(D2D1_POINT_2F point, D2D1_RECT_F rect);

	void Draw(S100DatasetManager& s100DatasetManager, S100PCManager& s100PCManager, ID2D1Factory1* pDirect2dFactory,  ID2D1DCRenderTarget* pRenderTarget, IDWriteTextFormat* pTextFormat, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyleS101Solid, ID2D1StrokeStyle1* pStrokeStyleS101Dash, AreaFillInfo& areaFillInfo, CRect& rectView);
	void DrawScale(ID2D1RenderTarget *pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat);

	FLOAT GetCurrentScale();
	FLOAT GetMaxZoom(); //ZOOM when it's 1:1000.

	void KeyDown(UINT nChar, CRect& rectView, int dpiX, int dpiY);

	void CreateBackgroundGeometry(ID2D1Factory1* pD2Factory);

	void GetMBRfromRenderTarget(D2D1::Matrix3x2F matrix, CRect& rect, FLOAT& xmin, FLOAT& ymin, FLOAT& xmax, FLOAT& ymax);
	bool IsIntersect(FLOAT xmin1, FLOAT ymin1, FLOAT xmax1, FLOAT ymax1, FLOAT xmin2, FLOAT ymin2, FLOAT xmax2, FLOAT ymax2);
	bool IsVisible(FLOAT currentScale, S101Dataset* s101Dataset);

	void SetViewFactor(double x, double y, long long zoom);
	void ZoomIn(double f = 1.1);
	void ZoomOut(double f = 0.9);
	void Move(double x, double y);
	void RotationClockwise();
	void RotationAntiClockwise();
	void InitViewFactor();
	void SetCenterPointByScreen(int x, int y);
	void SetOffset(int x, int y);
	void GetMapPointByScreenPoint(int sx, int sy, FLOAT* mx, FLOAT* my);
	void SetMapPointAtScreenPoint(FLOAT mx, FLOAT my, int sx, int sy);
};