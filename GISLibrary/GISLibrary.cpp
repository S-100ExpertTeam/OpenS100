#include "stdafx.h"
#include "PortrayalRenderCache.h"
#include "GISLibrary.h"
#include "Product.h"
#include "SGeometry.h"
#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "S10XGML.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <direct.h>

CGISLibraryApp::CGISLibraryApp()
{
	std::string str = "..\\TEMP"; // read path
	_mkdir(str.c_str()); //If it's new, 0, if it exists or if it's not, -1.

	m_pScaler = new Scaler();
	m_pCatalogManager = new CatalogManager();
	m_pLayerManager = new LayerManager(m_pScaler, m_pCatalogManager);
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

bool CGISLibraryApp::AddLayer(const std::wstring& _filepath)
{
	if (0 > m_pLayerManager->AddLayer(_filepath))
	{
		return false;
	}

	return true;
}

bool CGISLibraryApp::AddBackgroundLayer(const std::wstring& _filepath)
{
	return m_pLayerManager->AddBackgroundLayer(_filepath);
}

void CGISLibraryApp::Draw(HDC& hDC, int offset)
{
	m_pLayerManager->Draw(hDC, offset);
		
	//if (D2)
	//{
	//	CRect rectView = m_pScaler->GetScreenRect();
	//	D2->Begin(hDC, rectView);
	//	DrawS100Symbol(101, L"NORTHAR1", 30, 50, 0);
	//	DrawScaleBar();
	//	D2->End();
	//}
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

void CGISLibraryApp::SetMapProjection(MapProjection value)
{
	if (value == ::GetMapProjection())
	{
		return;
	}

	// Where the view is, on the ground.
	double longitude = (m_pScaler->mxMin + m_pScaler->mxMax) / 2;
	double latitude = (m_pScaler->myMin + m_pScaler->myMax) / 2;
	inverseProjection(longitude, latitude);
	const int scale = m_pScaler->GetCurrentScale();

	::SetMapProjection(value);
	m_pScaler->UpdateLimits();
	m_pLayerManager->ReloadLayers();

	constexpr double polarRegion = 60.0;
	const bool north = value == MapProjection::PolarStereographicNorth;
	if (IsPolarProjection() && (north ? latitude < polarRegion : latitude > -polarRegion))
	{
		MBR region(-180.0, north ? polarRegion : -90.0, 180.0, north ? 90.0 : -polarRegion);
		projection(region);
		m_pScaler->SetMap(region);
		m_pScaler->AdjustScreenMap();
	}
	else
	{
		projection(longitude, latitude);
		m_pScaler->SetMap(longitude, latitude, longitude, latitude);
		m_pScaler->SetScale(scale);
	}
}

std::wstring CGISLibraryApp::GetLayerName(int index)
{
	return m_pLayerManager->GetLayerName(index);
}
bool CGISLibraryApp::IsOn(int index)
{
	return m_pLayerManager->IsOn(index);
}
void CGISLibraryApp::DeleteLayer(int index)
{
	m_pLayerManager->DeleteLayer(index);
}
void CGISLibraryApp::DeleteLayer(const std::wstring& filepath)
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

/*
** screen setting
*/
void CGISLibraryApp::SetScreen(RECT* rect)
{
	if (rect == nullptr) return;
	SetScreen(*rect);
}

void CGISLibraryApp::SetScreen(const RECT& rect)
{
	m_pScaler->SetScreen(rect.left, rect.top, rect.right, rect.bottom);
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


// The display font is read from ENCCommon when a frame is drawn; nothing is kept here.
void CGISLibraryApp::ChangeDisplayFont()
{
}
