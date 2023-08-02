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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGISLibraryApp::CGISLibraryApp()
{
	gisLib = this;

	D2.CreateDeviceIndependentResources();
	D2.CreateDeviceDependentResources();

	//SCurve curve;
	//curve.Init(3);
	//curve.Set(0, 1, 2);
	//curve.Set(1, 2, 3);
	//curve.Set(2, 3, 4);
	//

	//std::string wkb_string = "010300000003000000050000000000000000003e400000000000002440000000000000244000000000000034405a1663db5d0d1440c1c69400de644540000000000000444000000000000044400000000000003e40000000000000244005000000e0bccaaf2fa62340088b1d196c384340ced595fa90862d40983dcea891d43540f04766949a083b40e07fa56f17a93340ea0954da635a3d403d6dbabf422c4240e0bccaaf2fa62340088b1d196c38434005000000168fd569e8c83f40c07544d9e0784240f0fb5eb0841c3f4023d071607c7b3f4002814e619fe9414084af9186c8004040d1bdba17e4cc4140858293ffcdeb4240168fd569e8c83f40c07544d9e0784240";
	//SSurface surface;
	//SCurve compCurve;
	//auto wkb = LatLonUtility::HexStringToWKB(wkb_string);
	//surface.ImportFromWkb(wkb, wkb_string.size() / 2);
	//char* a = nullptr;
	//int size = 0;
	//surface.ExportToWkb(&a, &size);

	//CString str;
	//for (int i = 0; i < size; i++)
	//{
	//	str.AppendFormat(_T("%02X"), a[i] & 0xff);
	//}
	//OutputDebugString(str);

	//delete[] wkb;
	//compCurve.Release();
	//SMultiPoint mp;
	//mp.Add(1, 2, 3);
	//mp.Add(4, 5, 6);
	//mp.Add(7, 8, 9);

	//

	////SPoint p;
	////p.SetPoint(123, 456);
	//char* a = nullptr;
	//int size = 0;

	//GeoPointZ p;
	//p.SetPoint(123, 456, 789);

	//curve.ExportToWkb(&a, &size);
	//curve.WriteWkb(L"G:\\TDS\\WKBLineString.wkb");

	//CString str;
	//for (int i = 0; i < size; i++)
	//{
	//	str.AppendFormat(_T("%02X"), a[i] & 0xff);
	//}
	//OutputDebugString(str);


	//mp.ExportToWkb(&a, &size);
	//mp.WriteWkb(L"G:\\TDS\\a.wkb");
	//auto wkb_mp = LatLonUtility::HexStringToWKB(wkb_string);


	//for (int i = 0; i < size; i++)
	//{
	//	str.AppendFormat(_T("%02X"), a[i] & 0xff);
	//}
	//OutputDebugString(str);

	//p.ExportToWkb(&a, &size);

	//for (int i = 0; i < size; i++)
	//{
	//	str.AppendFormat(_T("%02X"), a[i] & 0xff);
	//}
	//OutputDebugString(str);


	//delete[] a;


	//auto wkb = LatLonUtility::HexStringToWKB(wkb_string);
	//p.ImportFromWkb(wkb, 21);
	//delete[] wkb;

	//p.ExportToWkb(&a, &size);
	//delete[] a;

	m_pLayerManager->scaler = m_pScaler;
}

CGISLibraryApp::~CGISLibraryApp()
{
	D2.DeleteDeviceDependentResources();
	D2.DeleteDeviceIndependentResources();

	if (m_pScaler) {
		delete m_pScaler;
		m_pScaler = nullptr;
	}
	
	if (m_pLayerManager) {
		delete m_pLayerManager;
		m_pLayerManager = nullptr;
	}

	if (SGeometry::viewPoints) {
		delete[] SGeometry::viewPoints;
		SGeometry::viewPoints = nullptr;
	}

	featureOnOffMap.clear();
}

//CGISLibraryApp* gisLib = new CGISLibraryApp();

Scaler* CGISLibraryApp::GetScaler()
{
	return m_pScaler;
}

//void CGISLibraryApp::InitLibrary(std::wstring fcPath, std::wstring pcPath)
//{		
//	catalogManager.addFC(fcPath);
//	auto pc = catalogManager.addPC(pcPath);
//
//	if (pc) {
//		pc->CreateSVGD2Geometry(D2.pD2Factory);
//		pc->CreatePatternImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//		pc->CreateLineImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//	}
//}
//
//void CGISLibraryApp::InitLibrary(FeatureCatalogue* fc, PortrayalCatalogue* pc)
//{
//	catalogManager.addFC(fc);
//	catalogManager.addPC(pc);
//
//	if (pc) {
//		pc->CreateSVGD2Geometry(D2.pD2Factory);
//		pc->CreatePatternImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//		pc->CreateLineImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//	}
//}
//
//void CGISLibraryApp::addCatalogue(std::string fcPath, std::string pcPath)
//{
//	catalogManager.addFC(fcPath);
//	auto pc = catalogManager.addPC(pcPath);
//
//	if (pc) {
//		pc->CreateSVGD2Geometry(D2.pD2Factory);
//		pc->CreatePatternImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//		pc->CreateLineImages(D2.pD2Factory, D2.pImagingFactory, D2.D2D1StrokeStyleGroup.at(0));
//	}
//}
//
//void CGISLibraryApp::addCatalogue(std::wstring fcPath, std::wstring pcPath)
//{
//	addCatalogue(LibMFCUtil::WStringToString(fcPath), LibMFCUtil::WStringToString(pcPath));
//}

CatalogManager* CGISLibraryApp::getCatalogManager()
{
	return &catalogManager;
}

//FeatureCatalogue* CGISLibraryApp::addFC(std::string path)
//{
//	return catalogManager.addFC(path);
//}
//
//FeatureCatalogue* CGISLibraryApp::addFC(std::wstring path)
//{
//	return catalogManager.addFC(path);
//}
//
//PortrayalCatalogue* CGISLibraryApp::addPC(std::string path)
//{
//	return catalogManager.addPC(path);
//}
//
//PortrayalCatalogue* CGISLibraryApp::addPC(std::wstring path)
//{
//	return catalogManager.addPC(path);
//}

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

Layer* CGISLibraryApp::GetLayer(int index)
{
	return m_pLayerManager->GetLayer(index);
}

void CGISLibraryApp::DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale)
{
	auto pc = catalogManager.getPC(productNumber);
	if (pc)
	{
		auto pcManager = pc->GetS100PCManager();
		if (pcManager)
		{
			auto rt = D2.RenderTarget();
			auto brush = D2.SolidColorBrush();
			auto stroke = D2.SolidStrokeStyle();

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
		//OutputDebugString(errMsg);
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
		//OutputDebugString(errMsg);
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

//FeatureCatalogue* CGISLibraryApp::GetFC()
//{
//	return fc;
//}
//
//void CGISLibraryApp::SetFC(FeatureCatalogue* fc)
//{
//	this->fc = fc;
//	InitFeatureOnOffMap();
//}
//
//PortrayalCatalogue* CGISLibraryApp::GetPC()
//{
//	return pc;
//}
//
//void CGISLibraryApp::SetPC(PortrayalCatalogue* pc)
//{
//	this->pc = pc;
//}

void CGISLibraryApp::SetS100Scale(double value)
{
	s100Scale = value;
}

int CGISLibraryApp::GetS100Scale()
{
	if (s100Scale > 0)
	{
		return (int)s100Scale;
	}

	auto currentScale = m_pScaler->GetCurrentScale();

	return currentScale;
}


void CGISLibraryApp::InitFeatureOnOffMap()
{
	auto fc = catalogManager.getFC("S-101");
	if (fc)
	{
		featureOnOffMap.clear();

		auto vector = fc->GetFeatureTypes()->GetVecFeatureType();
		for (auto i = vector.begin(); i != vector.end(); i++)
		{
			featureOnOffMap.insert({ (*i)->GetCodeAsWString(), true });
		}
	}
}

void CGISLibraryApp::SetFeatureOnOff(std::wstring code, bool on)
{
	auto itor = featureOnOffMap.find(code);
	if (itor != featureOnOffMap.end())
	{
		itor->second = on;
	}
	else
	{
		featureOnOffMap.insert({ code, on });
	}
}

bool CGISLibraryApp::IsFeatureOn(std::wstring& featureTypeCode)
{
	auto item = featureOnOffMap.find(featureTypeCode);
	if (item != featureOnOffMap.end())
	{
		return item->second;
	}

	return true;
}