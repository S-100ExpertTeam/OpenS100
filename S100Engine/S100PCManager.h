#pragma once

#include <string>
#include "S100ColorProfile.h"
#include "S100ColorProfiles.h"
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
	S100ColorProfiles s100ColorProfiles;
	S100SymbolManager s100SymbolManager;
	std::vector<AreaFill> areaFills;
	AreaFillInfo areaFillInfo;
	LineStylesPackage::LineStyles lineStyles;
	CMap<CString, LPCTSTR, IWICBitmap*, IWICBitmap*&> lineImageMap;

private:
	char* ConvertWCtoC(wchar_t* str);

public:
	// Open single color profile
	bool AddS100ColorProfile(const std::string& id, const std::string& path);
	bool AddS100ColorProfile(const std::wstring& id, const std::wstring& path);

	// Open single svg
	bool AddS100Symbol(const std::string& path);
	bool AddS100Symbol(const std::wstring& path);
	bool OpenS100Symbol(const std::string& _path);
	bool OpenS100Symbol(const std::wstring& _path);

	// Open single line style
	bool AddS100LineStyle(const std::string& path);
	bool AddS100LineStyle(const std::wstring& path);
	bool OpenS100LineStyles(const std::string& _path);
	bool OpenS100LineStyles(const std::wstring& _path);

	// Open single area fill
	bool AddS100AreaFill(const std::string& path);
	bool AddS100AreaFill(const std::wstring& path);
	bool OpenS100AreaFills(const std::string& _path);
	bool OpenS100AreaFills(const std::wstring& _path);

	void Draw(const std::string& _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, D2D1_POINT_2F point, FLOAT rotation = 0, FLOAT scale = 5, const std::string& paletteName = "Day");
	void DrawAreaFill(const std::string& _symbolName, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, const std::string& paletteName = "Day");
	void DrawAreaFill(AreaFill* areaFill, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, const std::string& paletteName = "Day");
	void DrawLineStyle(const std::string& _name, ID2D1RenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush, ID2D1StrokeStyle1* pStrokeStyle, const std::string& paletteName = "Day");
	void CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory);
	void CreateBitmapImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, const std::string& paletteName = "Day");
	AreaPatternBitmap* CreateBitmapImage(AreaFill* pAreaFill, ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, const std::string& paletteName = "Day");
	void CreateBitmapBrush(ID2D1RenderTarget* pRenderTarget);
	ID2D1BitmapBrush* CreateBitmapBrush(AreaPatternBitmap* patternBitmap, ID2D1RenderTarget* pRenderTarget);
	void CreateLineImage(ID2D1Factory1* pDirect2dFactory, IWICImagingFactory* pImagingFactory, ID2D1StrokeStyle1* pStrokeStyleS101Solid, const std::string& paletteName = "Day");
	void DeleteBitmapBrush();
	void DeleteBitmapImage();
	void DeleteLineImage();
	void InverseMatrixBitmapBrush(D2D1::Matrix3x2F matrix);

	AreaFill* GetAreaFill(const std::string& _name);
	AreaFill* GetAreaFill(const std::wstring& _name);
	D2D1_RECT_F GetAreaFillRect(AreaFill* pAreaFill, FLOAT scale);
	D2D1_RECT_F GetLineStyleRect(const std::string& _name, FLOAT scale);
	D2D1_RECT_F GetLineStyleRect(const std::wstring& _name, FLOAT scale);

	IWICBitmap* GetLineImage(const std::string& key);
	IWICBitmap* GetLineImage(const std::wstring& key);
	
	// Safe pointer.
	S100ColorProfile* GetS100ColorProfile();
};