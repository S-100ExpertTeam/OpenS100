#include "stdafx.h"
#include "GISLibrary.h"
#include "Product.h"
#include "SGeometry.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "S10XGML.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGISLibraryApp::CGISLibraryApp()
{
	std::string str = "..\\TEMP"; // read path
	int mkFlag = _mkdir(str.c_str()); //If it's new, 0, if it exists or if it's not, -1.

	D2 = new GISLibrary::D2D1Resources();
	D2->CreateDeviceIndependentResources();
	D2->CreateDeviceDependentResources();

	m_pScaler = new Scaler();
	m_pCatalogManager = new CatalogManager(D2);
	m_pLayerManager = new LayerManager(m_pScaler, m_pCatalogManager, D2);
}

CGISLibraryApp::~CGISLibraryApp()
{
	if (m_pScaler) {
		delete m_pScaler;
		m_pScaler = nullptr;
	}

	if (m_pCatalogManager) {
		delete m_pCatalogManager;
		m_pCatalogManager = nullptr;
	}

	if (m_pLayerManager) {
		delete m_pLayerManager;
		m_pLayerManager = nullptr;
	}

	if (D2)
	{
		D2->DeleteDeviceDependentResources();
		D2->DeleteDeviceIndependentResources();
		delete D2;
		D2 = nullptr;
	}

	if (SGeometry::viewPoints) {
		delete[] SGeometry::viewPoints;
		SGeometry::viewPoints = nullptr;
	}
}

Scaler* CGISLibraryApp::GetScaler()
{
	return m_pScaler;
}

LayerManager* CGISLibraryApp::GetLayerManager()
{
	return m_pLayerManager;
}

CatalogManager* CGISLibraryApp::GetCatalogManager()
{
	return m_pCatalogManager;
}

D2D1Resources* CGISLibraryApp::GetD2D1Resources()
{
	return D2;
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

void CGISLibraryApp::Draw(HDC& hDC, int offset)
{
	m_pLayerManager->Draw(hDC, offset);
		
	if (D2)
	{
		CRect rectView = m_pScaler->GetScreenRect();
		D2->Begin(hDC, rectView);
		DrawS100Symbol(101, L"NORTHAR1", 30, 50, 0);
		DrawScaleBar();
		D2->End();
	}
}

void CGISLibraryApp::BuildPortrayalCatalogue(Layer* l)
{
	m_pLayerManager->BuildPortrayalCatalogue(l);
}

void CGISLibraryApp::S101RebuildPortrayal()
{
	m_pLayerManager->S101RebuildPortrayal();
}

Layer* CGISLibraryApp::GetLayer(int index)
{
	return m_pLayerManager->GetLayer(index);
}

void CGISLibraryApp::DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale)
{
	auto pc = m_pCatalogManager->getPC(productNumber);
	
	if (pc && D2)
	{
		auto pcManager = pc->GetS100PCManager();
		if (pcManager)
		{
			auto rt = D2->RenderTarget();
			auto brush = D2->SolidColorBrush();
			auto stroke = D2->SolidStrokeStyle();

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
void CGISLibraryApp::DeviceToWorld(long sx, long sy, double* mx, double* my, bool rotate)
{
	m_pScaler->DeviceToWorld(sx, sy, mx, my, rotate);
}

void CGISLibraryApp::WorldToDevice(double mx, double my, long* sx, long* sy, bool rotate)
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
void CGISLibraryApp::SetScreen(RECT* rect)
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

void CGISLibraryApp::GetMap(MBR* mbr)
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
void CGISLibraryApp::SetScale(int scale)
{
	m_pScaler->SetScale(scale);
}
void CGISLibraryApp::UpdateScale()
{
	m_pScaler->UpdateScale();
}

/*
** zoom in & zoom out
** factor : %
** ex : If you want to zoom in 20%, insert "20" into the factor.
*/
void CGISLibraryApp::ZoomIn(double value)
{
	m_pScaler->ZoomIn(value);
}
void CGISLibraryApp::ZoomIn(double value, int x, int y)
{
	m_pScaler->ZoomIn(value, x, y);
}
void CGISLibraryApp::ZoomOut(double value)
{
	m_pScaler->ZoomOut(value);
}
void CGISLibraryApp::ZoomOut(double value, int x, int y)
{
	m_pScaler->ZoomOut(value, x, y);
}
void CGISLibraryApp::AdjustScreenMap()
{
	m_pScaler->AdjustScreenMap();
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

void CGISLibraryApp::ChangeDisplayFont()
{	
	if (!D2)
		return;

	SafeRelease(&D2->pDWriteTextFormat);
	SafeRelease(&D2->pDWriteTextFormatArea);

	// Create a DirectWrite text format object.
	HRESULT hr = D2->pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2->pDWriteTextFormat
	);

	D2->pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2->pDWriteTextFormat)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat! (%02X)"), hr);
		//OutputDebugString(errMsg);
	}

	// Create a DirectWrite text format object.
	hr = D2->pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2->pDWriteTextFormatArea
	);

	D2->pDWriteTextFormatArea->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	D2->pDWriteTextFormatArea->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	D2->pDWriteTextFormatArea->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2->pDWriteTextFormatArea)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat (for Area)! (%02X)"), hr);
		//OutputDebugString(errMsg);
	}
}

std::wstring CGISLibraryApp::GetColorTable()
{
	if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Day)
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

