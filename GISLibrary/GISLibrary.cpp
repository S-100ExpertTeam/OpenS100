#include "stdafx.h"
#include "GISLibrary.h"

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

	delete fc;
	delete pc;
}

CGISLibraryApp* gisLib = new CGISLibraryApp();

Scaler* CGISLibraryApp::GetScaler()
{
	return m_pScaler;
}

void CGISLibraryApp::InitLibrary()
{
	D2.CreateDeviceIndependentResources();
	D2.CreateDeviceDependentResources();
	InitS100Engine();

	fc = new FeatureCatalogue(L"..\\ProgramData\\xml\\S-101_FC.xml");
	pc = new PortrayalCatalogue(L"..\\ProgramData\\S101_Portrayal\\");
	pc->CreateSVGD2Geometry(gisLib->D2.pD2Factory);
	pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
	pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
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

void CGISLibraryApp::BuildPortrayalCatalogue(Layer* l)
{
	m_pLayerManager->BuildPortrayalCatalogue(l);
}

void CGISLibraryApp::S101RebuildPortrayal()
{
	m_pLayerManager->S101RebuildPortrayal();
}

Layer* CGISLibraryApp::GetLayer()
{
	return m_pLayerManager->GetLayer();
}

void CGISLibraryApp::DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale)
{
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

CString CGISLibraryApp::GetLayerName()
{
	return m_pLayerManager->GetLayerName();
}
BOOL CGISLibraryApp::IsOn()
{
	return m_pLayerManager->IsOn();
}
void CGISLibraryApp::DeleteLayer()
{
	m_pLayerManager->DeleteLayer();
}
void CGISLibraryApp::DeleteLayer(CString filepath)
{
	m_pLayerManager->DeleteLayer(filepath);
}

void CGISLibraryApp::ReMBR()
{
	m_pLayerManager->ReMBR();
}

void CGISLibraryApp::SetViewMBR(RECT r)
{
	m_pLayerManager->SetViewMBR(r);
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

bool CGISLibraryApp::PtInMap(double _x, double _y)
{
	return m_pScaler->PtInMap(_x, _y);
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

FeatureCatalogue* CGISLibraryApp::GetFC()
{
	return fc;
}

PortrayalCatalogue* CGISLibraryApp::GetPC()
{
	return pc;
}