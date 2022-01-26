#include "stdafx.h"
#include "GISLibrary.h"
#include "ClipperUtil.h"

#include "..\\GeoMetryLibrary\\ENCCommon.h"
#include "..\\GeoMetryLibrary\\ENCGeometry.h"

#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

#include "..\\S100Geometry\\SGeometry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGISLibraryApp::CGISLibraryApp()
{
	m_pLayerManager->scaler = m_pScaler;
}

CGISLibraryApp::~CGISLibraryApp()
{
	D2.DeleteDeviceDependentResources();
	D2.DeleteDeviceIndependentResources();

	delete m_pScaler;
	m_pScaler = nullptr;

	delete m_pLayerManager;
	m_pLayerManager = nullptr;

	delete[] SGeometry::viewPoints;
	SGeometry::viewPoints = nullptr;
}

CGISLibraryApp* gisLib = new CGISLibraryApp();

Scaler* CGISLibraryApp::GetScaler()
{
	return m_pScaler;
}

void CGISLibraryApp::InitLibrary()
{
	D2.CreateDeviceIndependentResources();
	D2ChartResources.SetChartResources(&gisLib->D2);
	D2.CreateDeviceDependentResources();
	InitS100Engine();

	BasicFile S_101;
	S_101.Set_BasicFile(L"..\\ProgramData\\xml\\S-101_FC.xml" , L"..\\ProgramData\\S101_Portrayal\\");
	BasicFilePath.push_back(S_101);

	BasicFileSetting();
}

bool CGISLibraryApp::AddLayer(CString _filepath)
{
	m_pLayerManager->AddLayer(_filepath);
	return true;
}

bool CGISLibraryApp::AddBackgroundLayer(CString _filepath)
{
	return m_pLayerManager->AddBackgroundLayer(_filepath);
}

void CGISLibraryApp::Draw(HDC &hDC, int offset)
{
	m_pLayerManager->Draw(hDC, offset);
}

void CGISLibraryApp::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, double offsetX, double offsetY)
{
	m_pLayerManager->Draw(pRenderTarget, pDXFactory, offsetX, offsetY);
}

void CGISLibraryApp::BuildPortrayalCatalogue(Layer* l)
{
	m_pLayerManager->BuildPortrayalCatalogue(l);
}

void CGISLibraryApp::S101RebuildPortrayal()
{
	m_pLayerManager->S101RebuildPortrayal();
}

void CGISLibraryApp::DrawValidationLayers(HDC &hDC, int offset)
{
	m_pLayerManager->DrawValidationLayers(hDC, offset);
}

void CGISLibraryApp::DrawOverlay(HDC &hDC, int type, int offset)
{
	m_pLayerManager->DrawOverlay(hDC, type, offset);
}

void CGISLibraryApp::DrawShipMonitoringLayer(HDC &hDC, int offset)
{
	m_pLayerManager->DrawShipMonitoringLayer(hDC, offset);
}

void CGISLibraryApp::GetLayer(int index, Layer *_layer)
{
	m_pLayerManager->GetLayer(index, _layer);
}

Layer* CGISLibraryApp::GetLayer(int index)
{
	return m_pLayerManager->GetLayer(index);
}

void CGISLibraryApp::DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale)
{
	auto pc = LayerManager::GetPC(productNumber);
	if (pc)
	{
		auto pcManager = pc->GetS100PCManager();
		if (pcManager)
		{
			auto rt = gisLib->D2.RenderTarget();
			auto brush = gisLib->D2.SolidColorBrush();
			auto stroke = gisLib->D2.SolidStrokeStyle();

			D2D1::Matrix3x2F oldTransform;
			rt->GetTransform(&oldTransform);

			pcManager->Draw(
				symbolName,
				rt,
				brush,
				stroke,
				D2D1::Point2F((FLOAT)screenX, (FLOAT)screenY),
				(FLOAT)rotation,
				scale,
				pc->GetCurrentPaletteName());

			rt->SetTransform(oldTransform);
		}
	}
}

void CGISLibraryApp::DrawScaleBar()
{
	auto scaler = m_pLayerManager->GetScaler();
	auto scale = scaler->GetCurrentScale();
	auto screenHeight = scaler->GetScreenHeight();

	int sx1 = 10;
	int sy1 = screenHeight - 10;

	// Starting point of scale bar (bottom)
	double x1 = 0;
	double y1 = 0;

	scaler->DeviceToWorld(sx1, sy1, &x1, &y1);
	inverseProjection(x1, y1);

	// The end point of the scale bar (top).
	double x2 = 0;
	double y2 = 0;

	// scaledistance
	double scalebarDistance = 1852;

	if (scale >= 80000)
	{
		scalebarDistance *= 10;
	}

	LatLonUtility::CalcRhumbLinePosByGeographicLib(x1, y1, 0, scalebarDistance, &x2, &y2);

	projection(x2, y2);

	// The end point of the scale bar (top).
	long sx2 = 0;
	long sy2 = 0;

	scaler->WorldToDevice(x2, y2, &sx2, &sy2);

	double originalSymbolHeight = 20; // scalebar symbol height
	double symbolScale = (sy1 - sy2) / originalSymbolHeight;

	if (scale < 80000)
	{
		DrawS100Symbol(101, L"SCALEB10", sx1, sy1, 0, (float)symbolScale);
	}
	else
	{
		DrawS100Symbol(101, L"SCALEB11", sx1, sy1, 0, (float)symbolScale);
	}
}

void CGISLibraryApp::ClearInformationLayer(int index)
{
	m_pLayerManager->ClearInformationLayer(index);
}
void CGISLibraryApp::ClearInformationLayer(CString filepath)
{
	m_pLayerManager->ClearInformationLayer(filepath);
}

void CGISLibraryApp::ClearAllInformationLayer()
{
	m_pLayerManager->ClearAllInformationLayer();
}

CString CGISLibraryApp::GetLayerName(int index)
{
	return m_pLayerManager->GetLayerName(index);
}
BOOL CGISLibraryApp::IsOn(int index)
{
	return m_pLayerManager->IsOn(index);
}
void CGISLibraryApp::DeleteLayer(int index)
{
	m_pLayerManager->DeleteLayer(index);

}
void CGISLibraryApp::DeleteLayer(CString filepath)
{
	m_pLayerManager->DeleteLayer(filepath);
}

void CGISLibraryApp::DeleteAllLayer()
{
	m_pLayerManager->DeleteAllLayer();
}

void CGISLibraryApp::ReMBR()
{
	m_pLayerManager->ReMBR();
}

void CGISLibraryApp::SaveLayer(CString filename, CString extend, int index)
{
	m_pLayerManager->SaveLayer(filename, extend, index);
}

void CGISLibraryApp::SaveLayer(CString filename, CString extend, Layer *pLayer)
{
	m_pLayerManager->SaveLayer(filename, extend, pLayer);
}

void CGISLibraryApp::SetViewMBR(RECT r)
{
	m_pLayerManager->SetViewMBR(r);
}

int CGISLibraryApp::GetLayerCount()
{
	return m_pLayerManager->GetLayerCount();
}

std::vector<Layer*> CGISLibraryApp::GetAllLayer()
{
	return m_pLayerManager->m_listLayer;
}

MBR* CGISLibraryApp::GetMBR()
{
	return &m_pLayerManager->mbr;
}

/*
** points change.
*/
void CGISLibraryApp::DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate)
{
	m_pScaler->DeviceToWorld(sx, sy, mx, my, rotate);
}

void CGISLibraryApp::WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate)
{
	m_pScaler->WorldToDevice(mx, my, sx, sy, rotate);
}

void CGISLibraryApp::WorldToDevice(D2D1_POINT_2F& p)
{
	long sx = 0;
	long sy = 0;
	m_pScaler->WorldToDevice(p.x, p.y, &sx, &sy);

	p.x = (float)sx;
	p.y = (float)sy;
}

void CGISLibraryApp::DeviceToWorld(D2D1_POINT_2F& p)
{
	double mx = 0;
	double my = 0;
	m_pScaler->DeviceToWorld((long)p.x, (long)p.y, &mx, &my);

	p.x = (float)mx;
	p.y = (float)my;
}

/*
** Calculating the distance.
*/
double CGISLibraryApp::GetDistanceScreen(int x1, int y1, int x2, int y2)
{
	return m_pScaler->GetDistanceScreen(x1, y1, x2, y2);
}

/*
** screen setting
*/
void CGISLibraryApp::SetScreen(RECT *rect)
{
	if (rect == nullptr) return;
	m_pScaler->SetScreen(rect);
}

void CGISLibraryApp::SetScreen(const RECT& rect)
{
	m_pScaler->SetScreen(rect);
}

int CGISLibraryApp::GetCenterXScreen()
{
	return (int)m_pScaler->sox;
}

int CGISLibraryApp::GetCenterYScreen()
{
	return (int)m_pScaler->soy;
}

/*
** Map's information shown on the screen.
*/
void CGISLibraryApp::SetMap(double xmin, double ymin, double xmax, double ymax)
{
	m_pScaler->SetMap(xmin, ymin, xmax, ymax);
}

void CGISLibraryApp::SetMap(MBR mbr)
{

	m_pScaler->SetMap(mbr);
}

double CGISLibraryApp::GetMapWidth()
{
	return m_pScaler->GetMapWidth();
}
double CGISLibraryApp::GetMapHeight()
{
	return m_pScaler->GetMapHeight();
}

void CGISLibraryApp::GetMap(MBR *mbr)
{
	m_pScaler->GetMap(mbr);
}

/*
** Scale info
*/
int CGISLibraryApp::GetCurrentScale()
{
	return m_pScaler->GetCurrentScale();
}
void   CGISLibraryApp::SetScale(int scale)
{
	m_pScaler->SetScale(scale);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void   CGISLibraryApp::UpdateScale()
{
	m_pScaler->UpdateScale();

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}

/*
** zoom in & zoom out
** factor : %
** ex : If you want to zoom in 20%, insert "20" into the factor.
*/
void CGISLibraryApp::ZoomIn(double value)
{
	m_pScaler->ZoomIn(value);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::ZoomIn(double value, int x, int y)
{
	m_pScaler->ZoomIn(value, x, y);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::ZoomOut(double value)
{
	m_pScaler->ZoomOut(value);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();

#endif
}
void CGISLibraryApp::ZoomOut(double value, int x, int y)
{
	m_pScaler->ZoomOut(value, x, y);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::AdjustScreenMap()
{
	m_pScaler->AdjustScreenMap();

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}

/*
** move screen 
*/
// Use it to move as many pixels as you want
void CGISLibraryApp::MoveMap(int x, int y)
{
	m_pScaler->MoveMap(x, y);
}

// Use to position a desired point (mx, my) at a desired location (sy, sy)
void CGISLibraryApp::MoveMap(int sx, int sy, double mx, double my)
{
	m_pScaler->MoveMap(sx, sy, mx, my);
}

/*
** rotation screen
*/
void CGISLibraryApp::Rotate(LONG *sx, LONG *sy, double degree)
{
	m_pScaler->Rotate(sx, sy, degree);
}
void CGISLibraryApp::RotateMap(double degree)
{
	m_pScaler->RotateMap(degree);
}	//Maintain the range of rotateDegrees to [0, 360]
void CGISLibraryApp::NorthUp()
{
	m_pScaler->NorthUp();
}

bool CGISLibraryApp::PtInMap(double _x, double _y)
{
	return m_pScaler->PtInMap(_x, _y);
}


int CGISLibraryApp::GetLayerIndex(CString pathName)
{
	return m_pLayerManager->GetLayerIndex(pathName);
}

void CGISLibraryApp::InitS100Engine()
{
	//The part where View's Direct2D resources are handed over to SVGmanager.
	auto svgManager = S100_SVG_D2D1_DLL::GetApp()->GetSVGManager();
	if (svgManager)
	{
		svgManager->SetD2Resource(gisLib->D2.pD2Factory, gisLib->D2.pRT, gisLib->D2.pBrush, gisLib->D2.D2D1StrokeStyleGroup[0]);
	}
}

LayerManager* CGISLibraryApp::GetLayerManager()
{
	return m_pLayerManager;
}


void CGISLibraryApp::ChangeDisplayFont()
{
	SafeRelease(&D2.pDWriteTextFormat);
	SafeRelease(&D2.pDWriteTextFormatArea);

	// Create a DirectWrite text format object.
	HRESULT hr = D2.pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2.pDWriteTextFormat
	);

	D2.pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2.pDWriteTextFormat)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat! (%02X)"), hr);
		OutputDebugString(errMsg);
	}

	auto fsfsf = ENCCommon::DISPLAY_FONT_NAME.c_str();

	// Create a DirectWrite text format object.
	hr = D2.pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2.pDWriteTextFormatArea
	);

	D2.pDWriteTextFormatArea->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	D2.pDWriteTextFormatArea->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	D2.pDWriteTextFormatArea->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2.pDWriteTextFormatArea)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat (for Area)! (%02X)"), hr);
		OutputDebugString(errMsg);
	}
}

void CGISLibraryApp::SetDrawBackground(bool on)
{
	m_pLayerManager->m_baseMapOn = on;
}
bool CGISLibraryApp::GetDrawBackground()
{
	return m_pLayerManager->m_baseMapOn;
}

void CGISLibraryApp::SetExPCPath(CString value)
{
	m_pLayerManager->SetExPCPath(value);
}

CString CGISLibraryApp::GetExPCPath()
{
	return m_pLayerManager->GetExPCPath();
}

void CGISLibraryApp::SetExFCPath(CString value)
{
	m_pLayerManager->SetExFCPath(value);
}

CString CGISLibraryApp::GetExFCPath()
{
	return m_pLayerManager->GetExFCPath();
}

void CGISLibraryApp::BasicFileSetting()
{
	// load FC/PC 
	if (!LayerManager::pPortrayalCatalogues) //The part where  add something like S101.
	{
		LayerManager::pS100Catalogs = new std::vector<FeatureCatalogue*>();
		LayerManager::pPortrayalCatalogues = new std::vector<PortrayalCatalogue*>();

		LayerManager::hash_FC = new std::unordered_map<std::wstring, FeatureCatalogue*>();
		LayerManager::hash_PC = new std::unordered_map<std::wstring, PortrayalCatalogue*>();

		for (unsigned i = 0; i < BasicFilePath.size(); i++)
		{
			//add change color
			auto fc = LayerManager::AddS100FC(BasicFilePath[i].Get_FcPath());
			if (GetColorTable() != L"")
			{
				auto pc = LayerManager::AddS100PC(fc, BasicFilePath[i].Get_PortrayalPath(), GetColorTable());
				pc->CreateSVGD2Geometry(gisLib->D2.pD2Factory);
				pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
				pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
			}
			else
			{
				auto pc = LayerManager::AddS100PC(fc, BasicFilePath[i].Get_PortrayalPath());
				pc->CreateSVGD2Geometry(gisLib->D2.pD2Factory);
				pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
				pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
			}

		}
	}
}

std::wstring CGISLibraryApp::GetColorTable()
{
	if (ENCCommon::m_eColorTable == GeoMetryLibrary ::ColorTable::Day)
	{
		return L"Day";
	}
	else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Dusk)
	{
		return L"Dusk";
	}
	else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Night)
	{
		return L"Night";
	}
	else
	{
		return L"";
	}
}

void CGISLibraryApp::SetOwnShipPosition(double lon, double lat)
{
	m_pLayerManager->SetOwnShipPosition(lon, lat);
}

void CGISLibraryApp::SetOwnShipSize(double length, double width)
{
	m_pLayerManager->SetOwnShipSize(length, width);
}

ID2D1PathGeometry* CGISLibraryApp::combine_twice_path_geometries(ID2D1Factory1*& srcfactory, ID2D1PathGeometry* geo1, ID2D1PathGeometry* geo2)
{
	ID2D1PathGeometry* path_geo_1 = NULL;
	srcfactory->CreatePathGeometry(&path_geo_1);
	ID2D1GeometrySink* cmpl_s1 = NULL;
	path_geo_1->Open(&cmpl_s1);
	geo1->CombineWithGeometry(geo2, D2D1_COMBINE_MODE_UNION, NULL, cmpl_s1);
	cmpl_s1->Close();
	cmpl_s1->Release();

	return path_geo_1;
}

double CGISLibraryApp::GetAngleFromLegs(double _heading1, double _heading2, bool _radian)
{
	double PI = acos(-1.0);

	if (!_radian) {
		PI *= RAD2DEG;
	}

	if (_heading1 > PI) {
		_heading1 -= PI;
	}

	if (_heading2 > PI) {
		_heading2 -= PI;
	}

	double result = PI - _heading1 + _heading2;

	if (result > PI) {
		result = (2 * PI) - result;
	}

	return abs(result);
}

bool CGISLibraryApp::IsRight(double _x1Leg1, double _y1Leg1, double _x2Leg1, double _y2Leg1,
	double _x1Leg2, double _y1Leg2, double _x2Leg2, double _y2Leg2)
{
	double x1 = _x2Leg1 - _x1Leg1;
	double y1 = _y2Leg1 - _y1Leg1;
	double x2 = _x2Leg2 - _x1Leg2;
	double y2 = _y2Leg2 - _y1Leg2;

	double det = (x1 * y2) - (y1 * x2);

	if (det < 0) {
		return true;
	}

	return false;
}