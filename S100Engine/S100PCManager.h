#pragma once

#include <string>
#include "S100ColorProfile.h"
#include "S100SymbolManager.h"
#include "AreaFill.h"
#include "LineStylesPackage.h"
#include "AreaFillInfo.h"

struct ID2D1StrokeStyle1;
class S100PCManager
{
public:
	S100PCManager();
	virtual ~S100PCManager();

public:
	S100ColorProfile s100ColorProfile;
	S100SymbolManager s100SymbolManager;
	std::vector<AreaFill> areaFills;
	AreaFillInfo areaFillInfo;
	LineStylesPackage::LineStyles lineStyles;
	CMap<CString, LPCTSTR, IWICBitmap*, IWICBitmap*&> lineImageMap;

private:
	char* ConvertWCtoC(wchar_t* str);

public:
	bool OpenS100ColorProfile(std::wstring _path);
	bool OpenS100Symbol(std::wstring _path);	
	bool OpenS100AreaFills(std::wstring _path);
	bool OpenS100LineStyles(std::wstring _path);
	void Draw(std::wstring& _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, D2D1_POINT_2F point, FLOAT rotation = 0, FLOAT scale = 5, std::wstring paletteName = L"Day");
	void DrawAreaFill(std::wstring _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, std::wstring paletteName = L"Day");
	void DrawLineStyle(std::wstring _name, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, std::wstring paletteName = L"Day");
	void CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory);
	void CreateBitmapImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, std::wstring paletteName = L"Day");
	void CreateBitmapBrush(ID2D1RenderTarget* pRenderTarget);
	void CreateLineImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, std::wstring paletteName = L"Day");
	void DeleteBitmapBrush();
	void DeleteBitmapImage();
	void DeleteLineImage();
	void InverseMatrixBitmapBrush(D2D1::Matrix3x2F matrix);

	AreaFill* GetAreaFill(std::wstring _name);
	D2D1_RECT_F GetAreaFillRect(AreaFill* pAreaFill, FLOAT scale);
	D2D1_RECT_F GetLineStyleRect(CString _name, FLOAT scale);
	
	IWICBitmap* GetLineImage(std::wstring& key);
	
	// Safe pointer.
	S100ColorProfile* GetS100ColorProfile();
};