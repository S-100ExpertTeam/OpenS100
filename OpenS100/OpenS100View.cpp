﻿#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "OpenS100.h"
#endif

#include "OpenS100Doc.h"
#include "OpenS100View.h"
#include "MainFrm.h"
#include "DialogViewNoGeometry.h"
#include "DialogViewInformationType.h"
#include "DialogDockLayerManager.h"
#include "ConfigrationDlg.h"
#include "DialogDockCurrentSelection.h"
#include "TestGISLibrary.h"

#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/Layer.h"
#include "../GISLibrary/R_FeatureRecord.h"
#include "../GISLibrary/CodeWithNumericCode.h"
#include "../GISLibrary/S101Creator.h"
#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/D2D1Resources.h"
#include "../GISLibrary/SPoint.h"
#include "../GISLibrary/SMultiPoint.h"
#include "../GISLibrary/SCompositeCurve.h"
#include "../GISLibrary/SSurface.h"
#include "../GISLibrary/SGeometricFuc.h"
#include "../GISLibrary/ExchangeCatalogue.h"

#include "../GISLibrary/IC_InteroperabilityCatalogue.h"
//#include "../GISLibrary/ExchangeCatalogue.h"
#include "../GISLibrary/TranslationPackageType.h"

#include "../GeoMetryLibrary/GeometricFuc.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/GeoPolyline.h"

#include "../LatLonUtility/LatLonUtility.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <locale>        
#include <codecvt>       
#include <d2d1_1.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <crtdbg.h>
#include <iostream>
#include <vector>
#include <future>

#include "../LatLonUtility/LatLonUtility.h"

#include "../LatLonUtility/Logger.h"
#include "../PortrayalCatalogue/AlertCatalog.h"
#include "sqlite3.h"


#include "../GISLibrary/Permit.h"
#include "DialogDatasetManager.h"

#include "DataSetManagerSupport.h"

#pragma comment(lib, "d2d1.lib")

using namespace LatLonUtility;

class CMainFrame;

IMPLEMENT_DYNCREATE(COpenS100View, CView)

BEGIN_MESSAGE_MAP(COpenS100View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenS100View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(T1, Load100File)
	ON_COMMAND(T2, RemoveLoadFile)
	ON_COMMAND(T3, MapPlus)
	ON_COMMAND(T4, MapMinus)
	ON_COMMAND(T5, MapDragSize)
	ON_COMMAND(T6, MapFill)
	ON_COMMAND(T7, NoGeometry)
	ON_COMMAND(T8, NoGeometryInfo)
	ON_COMMAND(T9, Setting)
	ON_COMMAND(T10, DatasetManager)
	//ON_COMMAND(T10, &COpenS100App::OnAppAbout)
	ON_WM_CREATE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCMOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



void LogCallback(const std::string& message, LogLevel level) {
	OutputDebugStringA(message.c_str());
}

COpenS100View::COpenS100View() 
{
	//Logger::GetInstance().Subscribe(LogLevel::Error, LogCallback, "Error1");
	//Logger::GetInstance().Subscribe(LogLevel::Debug, LogCallback, "Debug1");
	//Logger::GetInstance().Subscribe(LogLevel::General, LogCallback, "General1");

	theApp.pView = this;

	//S100::IC_InteroperabilityCatalogue* item = new S100::IC_InteroperabilityCatalogue();
	//item->Open("../ic.xml");

	////auto pc = new PortrayalCatalogue();
	////pc->Open(L"../pc.xml");

	//delete item;
	//delete ex;

	//S100::AlertCatalog* alt = new S100::AlertCatalog();
	//alt->Open("../AlertCatalog-S101.xml");


	//S100::ExchangeCatalogue* ex = new S100::ExchangeCatalogue();
	//ex->Open("C:/Users/isbaek/Desktop/S100TestSet/S-101-Test-Datasets-main/S100_ROOT/CATALOG.XML");
	//ex->Save("C:/Users/isbaek/Desktop/S100TestSet/S-101-Test-Datasets-main/S100_ROOT/CATALOG1.XML");


	//S100::Permit* pm = new S100::Permit();
	//pm->Open("../PERMIT.XML");
	//delete pm;
}

COpenS100View::~COpenS100View()
{
	SaveLastPosScale();
	theApp.SaveSettings();

	DeleteDCs();

	CoUninitialize();

	if (dialogNoGeometry)
	{
		delete dialogNoGeometry;
		dialogNoGeometry = nullptr;
	}

	if (dialogInformationType)
	{
		delete dialogInformationType;
		dialogInformationType = nullptr;
	}

	if (s101Creator)
	{
		delete s101Creator;
		s101Creator = nullptr;
	}
}

void COpenS100View::SaveLastPosScale()
{
	double scale = theApp.gisLib->GetCurrentScale();
	int sox = theApp.gisLib->GetCenterXScreen();
	int soy = theApp.gisLib->GetCenterYScreen();

	MBR mbr;
	theApp.gisLib->GetMap(&mbr);

	double mox = (mbr.xmax + mbr.xmin) / 2;
	double moy = (mbr.ymax + mbr.ymin) / 2;

	inverseProjection(mox, moy);

	CString strScale;
	CString strSox;
	CString strSoy;

	strScale.Format(_T("%lf\n"), scale);
	strSox.Format(_T("%lf\n"), mox);
	strSoy.Format(_T("%lf\n"), moy);

	CStdioFile file;
	if (file.Open(_T("../ProgramData/data/init.txt"), CFile::modeWrite | CFile::modeCreate))
	{
		file.WriteString(strScale);
		file.WriteString(strSox);
		file.WriteString(strSoy);
	}
}

BOOL COpenS100View::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

void COpenS100View::OnDraw(CDC* pDC)
{
	COpenS100Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	if (!pDC->GetSafeHdc())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	theApp.gisLib->SetViewMBR(rect);
	if (theApp.gisLib2) {
		theApp.gisLib2->SetViewMBR(rect);
	}

	CreateDCs(pDC, rect);

	if (m_bMoveStart)
	{
		transDC.FillSolidRect(rect, RGB(0xD3, 0xD3, 0xD3));

		// Draw the latest screen according to panning, excluding the elements that need to be fixed.
		transDC.BitBlt(rect.left - (m_sp.x - m_ep.x), rect.top - (m_sp.y - m_ep.y), rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);

		// Draw a picture on the view of the completed screen.
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &transDC, 0, 0, SRCCOPY);
	}
	else
	{
		if (m_bMapRefesh) // Re-drawing part with MapRefresh() (Including Invalidate())
		{
			//std::future<void> drawResult = std::async(std::launch::async, [this, rect]() {
			//	DrawFromMapRefresh(&map_dc, (CRect&)rect);

			//	m_strFormatedScale = theApp.gisLib->GetScaler()->GetFormatedScale();

			//	mem_dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &map_dc, 0, 0, SRCCOPY);
			//});

			//drawResult.get();
			//Invalidate();

			

			DrawFromMapRefresh(&map_dc, (CRect&)rect);

		/*	if (m_Ex) {
				m_Ex->DrawCoverage(theApp.gisLib->D2.pRT, theApp.gisLib->D2.pD2Factory, theApp.gisLib->GetScaler(), 0, 0);
			}*/

			m_strFormatedScale = theApp.gisLib->GetScaler()->GetFormatedScale();
		}

		mem_dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &map_dc, 0, 0, SRCCOPY);

		// The part where I draw again with Invalidate.
		DrawFromInvalidate(&mem_dc, rect);

		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);
	}
}

void COpenS100View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect viewRect;
	GetClientRect(viewRect);

	theApp.gisLib->SetScreen(viewRect);
	
	if (theApp.gisLib2) {
		theApp.gisLib2->SetScreen(viewRect);
	}

	DeleteDCs();

	m_bMapRefesh = true;
}

void COpenS100View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenS100View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void COpenS100View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void COpenS100View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void COpenS100View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	s100EditRender.SelectByScreen(point.x, point.y);
	Invalidate();
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void COpenS100View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

#ifdef _DEBUG
void COpenS100View::AssertValid() const
{
	CView::AssertValid();
}

void COpenS100View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenS100Doc* COpenS100View::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenS100Doc)));
	return (COpenS100Doc*)m_pDocument;
}
#endif //_DEBUG

void COpenS100View::Load100File()
{
	//load file
	TCHAR BASED_CODE szFilter[] = _T("All supported files (*.000, *.gml, *.h5, *.shp, *.xml)|*.000;*.gml;*.h5;*.shp;*.xml|")
		_T("All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, 
		szFilter, this);

	std::vector<std::wstring> failedFiles;

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		while (pos)
		{
			CString filePath = dlg.GetNextPathName(pos);
			if (false == theApp.gisLib->AddLayer(filePath))
			{
				failedFiles.push_back(std::wstring(filePath));
			}
			theApp.m_pDockablePaneLayerManager.UpdateList();	
		}

		MapRefresh();

		if (failedFiles.size() > 0)
		{
			std::wstring message = L"Failed to load the following files:\n";
			for (auto file : failedFiles)
			{
				message += file + L"\n";
			}
			AfxMessageBox(message.c_str());
		}
	}
}

// Trash button
void COpenS100View::RemoveLoadFile()
{
	int selectedLayerIndex = theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedLayerIndex();

	if (selectedLayerIndex < 0)
	{
		AfxMessageBox(L"Select a layer first.");
		return;
	}

	auto lm = theApp.gisLib->GetLayerManager(); 
	lm->DeleteLayer(selectedLayerIndex);
	theApp.m_pDockablePaneLayerManager.DeleteLayer();
	ClearPickReport();
	MapRefresh();
}

void COpenS100View::MapPlus()
{
	CRect rect;
	GetClientRect(rect);
	theApp.gisLib->ZoomIn(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	if (theApp.gisLib2) {
		theApp.gisLib2->ZoomIn(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	}
	MapRefresh();
}

void COpenS100View::MapMinus()
{
	CRect rect;
	GetClientRect(rect);
	theApp.gisLib->ZoomOut(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	if (theApp.gisLib2) {
		theApp.gisLib2->ZoomOut(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	}
	MapRefresh();
}

void COpenS100View::MapDragSize()
{
	(m_Icon == ZOOM_AREA) ? m_Icon = MOVE : m_Icon = ZOOM_AREA;
}

void COpenS100View::MapFill()
{
	int selectedLayerIndex = theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedLayerIndex();

	if (selectedLayerIndex < 0)
	{
		AfxMessageBox(L"Select a layer first.");
		return;
	}

	auto lm = theApp.gisLib->GetLayerManager();

	Layer *layer = theApp.gisLib->GetLayer(selectedLayerIndex);
	if (nullptr == layer)
	{
		return;
	}

	auto layerMBR = layer->GetMBR();
	if (layerMBR.IsEmpty())
	{
		layerMBR.Extent(0.001);
	}

	theApp.gisLib->SetMap(layerMBR);
	if (theApp.gisLib2) {
		theApp.gisLib2->SetMap(layerMBR);
	}

	MapRefresh();
}

void COpenS100View::NoGeometry()
{
	auto layer = GetCurrentLayer();
	if (layer == nullptr)
	{
		AfxMessageBox(L"Select a layer first.");
		return;
	}

	S101Cell* cell = (S101Cell*)layer->m_spatialObject;

	if (nullptr == dialogNoGeometry)
	{
		dialogNoGeometry = new CDialogViewNoGeometry(this);
		dialogNoGeometry->Create(IDD_DIALOG_NOGEOMETRY);
	}

	dialogNoGeometry->SetNoGeometryFeatureList(cell);
	dialogNoGeometry->ShowWindow(SW_SHOW);
}

void COpenS100View::NoGeometryInfo()
{
	auto layer = GetCurrentLayer();
	if (layer == nullptr)
	{
		AfxMessageBox(L"Select a layer first.");
		return;
	}

	auto s100so = (S100SpatialObject*)layer->m_spatialObject;

	if (nullptr == dialogInformationType)
	{
		dialogInformationType = new CDialogViewInformationType(this);
		dialogInformationType->Create(IDD_DIALOG_INFORMATIONTYPE);
	}

	dialogInformationType->SetInformationFeatureList(s100so);
	dialogInformationType->ShowWindow(SW_SHOW);
}

void COpenS100View::Setting()
{
	CConfigrationDlg dlg(this);

	auto fc = theApp.gisLib->GetCatalogManager()->getFC("S-101");

	if (m_systemFontList.size() == 0)
	{
		// <FONT LIST>
		IDWriteFactory* pDWriteFactory = theApp.gisLib->GetD2D1Resources()->pDWriteFactory;
		IDWriteFontCollection* pFontCollection = NULL;

		// Get the system font collection.
		HRESULT hr = pDWriteFactory->GetSystemFontCollection(&pFontCollection);
		UINT32 familyCount = 0;
		
		// Get the number of font families in the collection.
		if (SUCCEEDED(hr))
		{
			familyCount = pFontCollection->GetFontFamilyCount();
		}
		
		for (UINT32 i = 0; i < familyCount; ++i)
		{
			IDWriteFontFamily* pFontFamily = NULL;
			// Get the font family.
			if (SUCCEEDED(hr))
			{
				hr = pFontCollection->GetFontFamily(i, &pFontFamily);
				IDWriteLocalizedStrings* pFamilyNames = NULL;

				// Get a list of localized strings for the family name.
				if (SUCCEEDED(hr))
				{
					hr = pFontFamily->GetFamilyNames(&pFamilyNames);
					UINT32 index = 0;
					BOOL exists = false;

					wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

					if (SUCCEEDED(hr))
					{
						// Get the default locale for this user.
						int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

						// If the default locale is returned, find that locale name, otherwise use "en-us".
						if (defaultLocaleSuccess)
						{
							hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);
						}
						
						if (SUCCEEDED(hr) && !exists) // if the above find did not find a match, retry with US English
						{
							hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
						}

						UINT32 length = 0;

						// Get the string length.
						if (SUCCEEDED(hr))
						{
							hr = pFamilyNames->GetStringLength(index, &length);
						}

						// Allocate a string big enough to hold the name.
						wchar_t* name = new wchar_t[length + 1];
						if (name == NULL)
						{
							hr = E_OUTOFMEMORY;
						}

						// Get the family name.
						if (SUCCEEDED(hr))
						{
							hr = pFamilyNames->GetString(index, name, length + 1);

							m_systemFontList.push_back(name);
						}

						delete[] name;
					}
					
					// If the specified locale doesn't exist, select the first on the list.
					if (!exists)
						index = 0;
				}

				SafeRelease(&pFamilyNames);
			}
		}
		SafeRelease(&pFontCollection);
	}
	dlg.GetConfigPage1()->SetFontList(m_systemFontList);
	dlg.DoModal();
}


void COpenS100View::DatasetManager()
{
	DialogDatasetManager dlg;
	dlg.ct = theApp.gisLib->GetCatalogManager();
	dlg.DoModal();
}


void COpenS100View::MapRefresh()
{
	m_bMapRefesh = true;
	Invalidate(FALSE);
}

void COpenS100View::OpenWorldMap()
{
	theApp.gisLib->AddBackgroundLayer(_T("../ProgramData/World/World.shp"));
	//theApp.gisLib2->AddBackgroundLayer(_T("../ProgramData/World/World.shp"));

	MapRefresh();
}

int COpenS100View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// catalog manager
	auto cm = theApp.gisLib->GetCatalogManager();

	//import FC
	auto fclist = DataSetManagerSupport::GetInstance().initFCList();
	for (auto item : fclist)
	{
		cm->addFC(item);
	}

	//import PC
	auto pclist = DataSetManagerSupport::GetInstance().initPCList();
	for (auto item : pclist)
	{
		cm->addPC(item);
	}

	// import Dataset
	auto dsList = DataSetManagerSupport::GetInstance().initDataFileList();
	for (auto item : dsList)
	{
		theApp.gisLib->AddLayer(LibMFCUtil::StringToWString(item).c_str());
	}

	// import Support files
	auto supportFiles = DataSetManagerSupport::GetInstance().initSupportFiles();
	for (auto item : supportFiles)
	{
		CString cPath = LibMFCUtil::StringToWString(item).c_str();
		auto ext = LibMFCUtil::GetExtension(cPath);
		if (ext.CompareNoCase(L"xml") == 0)
		{
			TranslationPackageType languagePack;
			debug_print_utf8("Language: " + languagePack.Language + "\n");
			debug_print_utf8("Issue Date: " + languagePack.IssueDate + "\n");
			debug_print_utf8("Issue Time: " + languagePack.IssueTime + "\n");
			/*print_utf8("Responsible Party: " + translationPackage->ResponsibleParty + "\n");*/

			for (const auto& sourceFile : languagePack.SourceFiles) {
				debug_print_utf8("Source File Identifier: " + sourceFile.Header.ResourceIdentifier + "\n");
				for (const auto& id : sourceFile.Header.Identifications) {
					debug_print_utf8("  Path: " + id.Path + ", Value: " + id.Value + "\n");
				}
				for (const auto& item : sourceFile.TranslationItems) {
					debug_print_utf8("  Translation Item Path: " + item.Path + "\n");
					debug_print_utf8("    Original: " + item.Original + "\n");
					debug_print_utf8("    Status: " + item.ItemStatus.toString() + "\n");
					debug_print_utf8("    Translation: " + item.Translation + "\n");
				}
			}
		}
		//theApp.gisLib->AddLayer(LibMFCUtil::StringToWString(item).c_str());
	}

	return 0;
}


void COpenS100View::CreateDCs(CDC* pDC, CRect& rect)
{
	if (!mem_dc.GetSafeHdc())
	{
		if (mem_dc.CreateCompatibleDC(pDC))
		{
			if (memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				CBitmap* pOldBitmap = mem_dc.SelectObject(&memBitmap);
				if (pOldBitmap != NULL)
					pOldBitmap->DeleteObject();
			}
			mem_dc.SetBkMode(TRANSPARENT);
		}
	}

	if (!transDC.GetSafeHdc())
	{
		if (transDC.CreateCompatibleDC(pDC))
		{
			if (transBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				CBitmap* pOldBitmap = transDC.SelectObject(&transBitmap);
				if (pOldBitmap != NULL)
					pOldBitmap->DeleteObject();
			}
			transDC.SetBkMode(TRANSPARENT);
		}
	}

	if (!map_dc.GetSafeHdc())
	{
		if (map_dc.CreateCompatibleDC(pDC))
		{
			if (mapBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				CBitmap* pOldBitmap = map_dc.SelectObject(&mapBitmap);
				if (pOldBitmap != NULL)
					pOldBitmap->DeleteObject();
			}
			map_dc.SetBkMode(TRANSPARENT);
		}
	}
}

void COpenS100View::DeleteDCs()
{
	if (mem_dc.GetSafeHdc())
	{
		mem_dc.DeleteDC();
		memBitmap.DeleteObject();
	}

	if (transDC.GetSafeHdc())
	{
		transDC.DeleteDC();
		transBitmap.DeleteObject();
	}

	if (map_dc.GetSafeHdc())
	{
		map_dc.DeleteDC();
		mapBitmap.DeleteObject();
	}
}


void COpenS100View::OnMButtonDown(UINT nFlags, CPoint point)
{
	CView::OnMButtonDown(nFlags, point);
}


void COpenS100View::OnMButtonUp(UINT nFlags, CPoint point)
{
	//s100EditRender.UpdatePoint(point.x, point.y, theApp.gisLib->GetLayerManager());
	MapRefresh();
	CView::OnMButtonUp(nFlags, point);
}


void COpenS100View::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	CRect cr;
	GetClientRect(&cr);
	theApp.gisLib->DeviceToWorld(cr.Width() / 2, cr.Height() / 2, &moveMX, &moveMY);
	if (theApp.gisLib2) {
		theApp.gisLib2->DeviceToWorld(cr.Width() / 2, cr.Height() / 2, &moveMX2, &moveMY2);
	}

	m_sp = point;
	m_ep = point;
	isMoved = false;

	switch (m_Icon)
	{
	case ZOOM_AREA:
		m_ptStartZoomArea = point;
		m_bZoomArea = true;
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_AREA));
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}


void COpenS100View::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CRect cr;
	GetClientRect(&cr);

	CPoint calc_point = point;

	int dx = point.x - m_sp.x;
	int dy = point.y - m_sp.y;

	calc_point.x = dx + calc_point.x;
	calc_point.y = dy + calc_point.y;

	if (!(
		(m_Icon == ZOOM_AREA) ||
		(m_Icon == DISTANCE) ||
		(m_Icon == MEASURE_AREA)
		))
	{
		if (isMoved == false)
		{
			SetPick();
			PickReport(calc_point);
		}
	}

	if (isMoved == true)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		theApp.gisLib->MoveMap(cr.Width() / 2 + dx, cr.Height() / 2 + dy, moveMX, moveMY);
		
		if (theApp.gisLib2) {
			theApp.gisLib2->MoveMap(cr.Width() / 2 + dx, cr.Height() / 2 + dy, moveMX2, moveMY2);
		}

		MapRefresh();
	}
	else
	{
		switch (m_Icon)
		{
		case MOVE:
			break;
		case ZOOM_AREA:
		{
			m_Icon = MOVE;
			m_bZoomArea = false;
			m_ptEndZoomArea = calc_point;

			if ((m_ptStartZoomArea.x != m_ptEndZoomArea.x) &&
				(m_ptStartZoomArea.y != m_ptEndZoomArea.y))
			{
				double startX = 0;
				double startY = 0;
				double endX = 0;
				double endY = 0;

				theApp.gisLib->DeviceToWorld(
					m_ptStartZoomArea.x,
					m_ptStartZoomArea.y,
					&startX,
					&startY);

				theApp.gisLib->DeviceToWorld(
					m_ptEndZoomArea.x,
					m_ptEndZoomArea.y,
					&endX,
					&endY);

				double xmin = 0;
				double ymin = 0;
				double xmax = 0;
				double ymax = 0;

				if (startX < endX)
				{
					xmin = startX;
					xmax = endX;
				}
				else
				{
					xmin = endX;
					xmax = startX;
				}

				if (startY < endY)
				{
					ymin = startY;
					ymax = endY;
				}
				else
				{
					ymin = endY;
					ymax = startY;
				}

				theApp.gisLib->SetMap(xmin, ymin, xmax, ymax);

				CRect viewRect;
				GetClientRect(viewRect);

				theApp.gisLib->SetScreen(viewRect);
				theApp.gisLib->ZoomOut(0, viewRect.Width() / 2, viewRect.Height() / 2);
				theApp.gisLib->UpdateScale();

				theApp.gisLib->AdjustScreenMap();

				MapRefresh();
			}
			break;
		}
		}
	}

	Invalidate(FALSE);
	m_bMoveStart = FALSE;
	isMoved = false;

	CView::OnLButtonUp(nFlags, point);
}


void COpenS100View::OnMouseMove(UINT nFlags, CPoint point)
{
	this->SetFocus();
	m_ptCurrent = point;

	/*
	** Display the latitude value of the current mouse pointer position.
	*/
	double curLat, curLong;
	theApp.gisLib->DeviceToWorld(point.x, point.y, &curLong, &curLat);
	inverseProjection(curLong, curLat);

	// The X coordinate values are modified to be included within the ranges of (-180 and 180).
	if (curLong < -180) {
		curLong += 360;
	}

	if (curLong > 180) {
		curLong -= 360;
	}

	m_strLatitude.Format(_T("Lat : %lf"), curLat);
	m_strLongitude.Format(_T("Lon : %lf"), curLong);

	double degree, minute, second;

	degree = (int)curLong;
	minute = (curLong - degree) * 60;
	second = second = (minute - (int)minute) * 60;

	CString strLon = m_strLongitude;
	if (degree>=0) 
	{
		strLon.Format(_T("%3d-%.3lf(E)"), std::abs((int)degree),std::fabs(minute));
	}
	else {
		strLon.Format(_T("%3d-%.3lf(W)"), std::abs((int)degree), std::fabs(minute));
	}
	m_strFormatedLongitude = strLon;
	
	//====================================================================================
	// The X coordinate values are modified to be included within the ranges of (-180 and 180).
	if (curLat < -180) {
		curLat += 360;
	}

	if (curLat > 180) {
		curLat -= 360;
	}

	degree = (int)curLat;

	minute = (curLat - degree) * 60;

	second = (minute - (int)minute) * 60;


	CString strLat = m_strLatitude;
	if (degree>=0)
	{
		strLat.Format(_T("%2d-%.3lf(N)"), std::abs((int)degree), std::fabs(minute));
	}
	else {
		strLat.Format(_T("%2d-%.3lf(S)"), std::abs((int)degree), std::fabs(minute));
	}
	m_strFormatedLatitude = strLat;


	
	
	CString strFomatedInfo = _T("");
	strFomatedInfo.Format(_T("%s , %s , %s"), m_strFormatedScale, m_strFormatedLatitude, m_strFormatedLongitude);

	CMainFrame *frame = (CMainFrame*)AfxGetMainWnd(); 
	frame->SetMessageText(strFomatedInfo);


	CFont mFont3;

	//Create a font.
	mFont3.CreateFontW(
		20, // height font
		8, // width font
		0, // Print angle.
		0, // angle from the baseline.
		FW_HEAVY, // Thickness of letters.
		FALSE, // Italic
		FALSE, // underline
		FALSE, // cancle line
		DEFAULT_CHARSET, // character set 

		OUT_DEFAULT_PRECIS, // print precision.
		CLIP_CHARACTER_PRECIS, // Clipping precision.
		PROOF_QUALITY, // text quality.
		DEFAULT_PITCH, // font Pitch
		_T("Consolas") // font
	);

	frame->SetFont(&mFont3);
	m_ep = point;
	
	if (!(beModifyWaypoint ||
		ZOOM_AREA == m_Icon
		))
	{
		if (abs(m_sp.x - m_ep.x) + abs(m_sp.y - m_ep.y) > 3)
		{
			isMoved = true;
		}

		if ((nFlags & MK_LBUTTON))
		{
			m_bMoveStart = TRUE;
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_GRAB));
			Invalidate(FALSE);
		}
	}

	else
	{
		switch (m_Icon)
		{
		case MOVE:
			break;
		case DISTANCE:
			Invalidate(FALSE);
			break;
		case ZOOM_AREA:
		{
			if (m_bZoomArea)
			{
				m_ptEndZoomArea = point;
				Invalidate(FALSE);
			}
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_AREA));
			break;
		}
		}
	}
	UpdateData(FALSE);
	Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}


void COpenS100View::DrawZoomArea(CDC* pDC)
{
	if (!m_bZoomArea)
	{
		return;
	}


	CRect rect;

	if (m_ptStartZoomArea.x < m_ptEndZoomArea.x)
	{
		rect.left = m_ptStartZoomArea.x;
		rect.right = m_ptEndZoomArea.x;
	}
	else
	{
		rect.left = m_ptEndZoomArea.x;
		rect.right = m_ptStartZoomArea.x;
	}

	if (m_ptStartZoomArea.y < m_ptEndZoomArea.y)
	{
		rect.top = m_ptStartZoomArea.y;
		rect.bottom = m_ptEndZoomArea.y;
	}
	else
	{
		rect.top = m_ptEndZoomArea.y;
		rect.bottom = m_ptStartZoomArea.y;
	}

	CPen newPen;
	CBrush newBrush;
	newPen.CreatePen(PS_DASH, 1, RGB(255, 51, 51));
	newBrush.CreateStockObject(NULL_BRUSH);

	CPen *oldPen = pDC->SelectObject(&newPen);
	CBrush *oldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(rect);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}


void COpenS100View::DrawFromMapRefresh(CDC* pDC, CRect& rect)
{
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));

	HDC hdc = pDC->GetSafeHdc();

	theApp.gisLib->Draw(hdc);

	if (theApp.gisLib2) {
		theApp.gisLib2->Draw(hdc);
	}

	m_bMapRefesh = false;
}

void COpenS100View::DrawFromInvalidate(CDC* pDC, CRect& rect)
{
	HDC hdc = pDC->GetSafeHdc();

	DrawZoomArea(pDC);
	DrawPickReport(hdc);

	
}

Layer* COpenS100View::GetCurrentLayer()
{
	int selectedLayerIndex = theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedLayerIndex();

	if (selectedLayerIndex < 0)
	{		
		return nullptr;
	}

	auto layer = theApp.gisLib->GetLayer(selectedLayerIndex);
	return layer;
}

BOOL COpenS100View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ((CWnd*)this != GetFocus())
	{
		return TRUE;
	}

	// When raising the mouse, => Zoom in.
	if (zDelta > 0)
	{
		theApp.gisLib->ZoomIn(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
		theApp.gisLib->AdjustScreenMap();
		if (theApp.gisLib2) {
			theApp.gisLib2->ZoomIn(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
			theApp.gisLib2->AdjustScreenMap();
		}
	}
	// When you lower the mouse wheel => Zoom out.
	else
	{
		theApp.gisLib->ZoomOut(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
		theApp.gisLib->AdjustScreenMap();
		if (theApp.gisLib2) {
			theApp.gisLib2->ZoomOut(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
			theApp.gisLib2->AdjustScreenMap();
		}
	}

	MapRefresh();

	m_strFormatedScale = theApp.gisLib->GetScaler()->GetFormatedScale();
	CString strFomatedInfo = _T("");
	strFomatedInfo.Format(_T("%s , %s , %s"), m_strFormatedScale, m_strFormatedLatitude, m_strFormatedLongitude);

	CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
	frame->SetMessageText(strFomatedInfo);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void COpenS100View::DrawPickReport(HDC& _hdc, int offsetX, int offsetY)
{
	if (encPick == nullptr)
	{
		return;
	}
	
	Graphics gPick(_hdc);
	auto frPick = encPick->GetFeatureType(pugi::as_utf8(featurePick));
	if (frPick && !frPick->IsNoGeometry())
	{
		DrawS101PickReport(gPick, offsetX, offsetY);
	}
}

void COpenS100View::DrawS101PickReport(Graphics& g, int offsetX, int offsetY)
{
	auto frPick = encPick->GetFeatureType(pugi::as_utf8(featurePick));

	// Point
	if (frPick->GetGeometry()->GetType() == SGeometryType::Point)
	{
		auto pt = (SPoint*)frPick->GetGeometry();

		long x = 0;
		long y = 0;

		theApp.gisLib->WorldToDevice(pt->x, pt->y, &x, &y);
		x += offsetX;
		y += offsetY;

		g.DrawLine(&Pen(Color(255, 0, 0), 4), x - 20, y + 8, x - 20, y + 20);
		g.DrawLine(&Pen(Color(255, 0, 0), 4), x - 20, y + 20, x - 8, y + 20);

		g.DrawLine(&Pen(Color(255, 0, 0), 4), x - 20, y - 8, x - 20, y - 20);
		g.DrawLine(&Pen(Color(255, 0, 0), 4), x - 20, y - 20, x - 8, y - 20);

		g.DrawLine(&Pen(Color(255, 0, 0), 4), x + 20, y + 8, x + 20, y + 20);
		g.DrawLine(&Pen(Color(255, 0, 0), 4), x + 20, y + 20, x + 8, y + 20);

		g.DrawLine(&Pen(Color(255, 0, 0), 4), x + 20, y - 8, x + 20, y - 20);
		g.DrawLine(&Pen(Color(255, 0, 0), 4), x + 20, y - 20, x + 8, y - 20);

	}
	else if (frPick->GetGeometry()->GetType() == SGeometryType::MultiPoint)
	{
		auto multiPoint = (SMultiPoint*)frPick->GetGeometry();

		for (int i = 0; i < multiPoint->GetNumPoints(); i++)
		{
			long x = 0;
			long y = 0;

			theApp.gisLib->WorldToDevice(multiPoint->GetX(i), multiPoint->GetY(i), &x, &y);
			x += offsetX;
			y += offsetY;

			g.DrawEllipse(&Pen(Color(255, 0, 0), 4), x - 20, y - 20, 40, 40);
		}
	}
	// Line
	else if (frPick->GetGeometry()->GetType() == SGeometryType::CompositeCurve)
	{
		SolidBrush brush(Color(255, 0, 0));

		auto cc = (SCompositeCurve*)(frPick->GetGeometry());

		auto curveCnt = cc->GetCurveCount();
		for (int i = 0; i < curveCnt; i++)
		{
			auto c = cc->GetCurve(i);
			Gdiplus::Point* pickPoints = new Gdiplus::Point[c->getNumPoint()];

			int pickNumPoints = 0;

			pickNumPoints = c->getNumPoint();

			for (auto i = 0; i < pickNumPoints; i++)
			{
				pickPoints[i].X = (INT)c->GetX(i);
				pickPoints[i].Y = (INT)c->GetY(i);
				theApp.gisLib->WorldToDevice(c->GetX(i), c->GetY(i),
					(long*)(&pickPoints[i].X), (long*)(&pickPoints[i].Y));

				pickPoints[i].X += offsetX;
				pickPoints[i].Y += offsetY;
			}

			g.DrawLines(&Pen(&brush, 4), pickPoints, pickNumPoints);

			delete[] pickPoints;
		}
	}
	else if (frPick->GetGeometry()->GetType() == SGeometryType::Curve)
	{
		SolidBrush brush(Color(255, 0, 0));

		auto c = (SCurve*)(frPick->GetGeometry());

		Gdiplus::Point* pickPoints = new Gdiplus::Point[c->m_numPoints];

		int pickNumPoints = 0;

		pickNumPoints = c->GetNumPoints();

		for (auto i = 0; i < pickNumPoints; i++)
		{
			pickPoints[i].X = (INT)c->m_pPoints[i].x;
			pickPoints[i].Y = (INT)c->m_pPoints[i].y;
			theApp.gisLib->WorldToDevice(c->m_pPoints[i].x, c->m_pPoints[i].y,
				(long*)(&pickPoints[i].X), (long*)(&pickPoints[i].Y));

			pickPoints[i].X += offsetX;
			pickPoints[i].Y += offsetY;
		}

		g.DrawLines(&Pen(&brush, 4), pickPoints, pickNumPoints);

		delete[] pickPoints;
	}
	// Area
	else if (frPick->GetGeometry()->GetType() == SGeometryType::Surface)
	{
		auto surface = (SSurface*)frPick->GetGeometry();
		auto geometry = surface->GetNewD2Geometry(theApp.gisLib->GetD2D1Resources()->pD2Factory, theApp.gisLib->GetScaler());

		D2D1_COLOR_F color{};
		color.r = 255 / float(255.0);
		color.g = 50 / float(255.0);
		color.b = 50 / float(255.0);
		color.a = float(0.7);

		HDC hdc = g.GetHDC();
		theApp.gisLib->GetD2D1Resources()->Begin(hdc, theApp.gisLib->GetScaler()->GetScreenRect());
		if (geometry)
		{
			theApp.gisLib->GetD2D1Resources()->pRT->SetTransform(D2D1::Matrix3x2F::Translation(float(offsetX), float(offsetY)));
			theApp.gisLib->GetD2D1Resources()->pBrush->SetColor(color);
			theApp.gisLib->GetD2D1Resources()->pBrush->SetOpacity(float(0.7));
			theApp.gisLib->GetD2D1Resources()->pRT->FillGeometry(geometry, theApp.gisLib->GetD2D1Resources()->pBrush);
			SafeRelease(&geometry);
		}
		theApp.gisLib->GetD2D1Resources()->End();
	}

	auto hdc = g.GetHDC();
	CRect rect = theApp.gisLib->GetScaler()->GetScreenRect();
	theApp.gisLib->GetD2D1Resources()->Begin(hdc, rect);
	
	if (encPick->GetProductNumber() == 101)
	{
		s100EditRender.Set((S101Cell*)encPick, theApp.gisLib->GetScaler(), (R_FeatureRecord*)frPick);
		s100EditRender.ShowPoint();
	}
	theApp.gisLib->GetD2D1Resources()->End();
	g.ReleaseHDC(hdc);
}

void COpenS100View::ClearPickReport()
{
	SetPick();
}

void COpenS100View::PickReport(CPoint _point)
{
	auto layerCount = theApp.gisLib->GetLayerManager()->LayerCount();

	if (1 == layerCount)
	{
		PickReport(_point, 0);
	}
	else
	{
		auto selectedLayerIndex = theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedLayerIndex();

		if (selectedLayerIndex < 0 || selectedLayerIndex >> theApp.gisLib->GetLayerManager()->LayerCount())
		{
			return;
		}

		PickReport(_point, selectedLayerIndex);
	}
}

void COpenS100View::PickReport(CPoint _point, int layerIndex)
{
	auto layer = theApp.gisLib->GetLayerManager()->GetLayer(layerIndex);
	if (nullptr == layer || !layer->IsS100Layer())
	{
		return;
	}

	auto s100layer = (S100Layer*)layer;

	auto cell = s100layer->GetS100SpatialObject();
	if (nullptr == cell)
	{
		return;
	}

	CString featureType = L"Feature";
	CStringArray csa;

	double xmin = 0;
	double ymin = 0;
	double xmax = 0;
	double ymax = 0;

	theApp.gisLib->DeviceToWorld(_point.x - 5, _point.y + 5, &xmin, &ymin);  // start point
	theApp.gisLib->DeviceToWorld(_point.x + 5, _point.y - 5, &xmax, &ymax);  // end point

	MBR pickMBR(xmin, ymin, xmax, ymax);

	__int64 key = 0;
	R_FeatureRecord* fr = nullptr;

	auto featureCount = cell->GetFeatureCount();
	for (int i = 0; i < featureCount; i++) {
		auto fr = cell->GetFeatureTypeByIndex(i);
		if (fr->geometry == nullptr || fr->geometry->GetType() != SGeometryType::Surface)
		{
			continue;
		}

		SSurface* surface = (SSurface*)fr->geometry;

		if (MBR::CheckOverlap(pickMBR, fr->geometry->m_mbr))
		{
			double centerX = 0;
			double centerY = 0;

			theApp.gisLib->DeviceToWorld(_point.x, _point.y, &centerX, &centerY);

			if (SGeometricFuc::inside(centerX, centerY, (SSurface*)surface) == 1)
			{
				CString csFrid;
				CString csFoid;
				CString csLat;
				CString csLon;
				CString csType;
				CString csName;
				CString csAssoCnt;

				double lon = surface->GetX();
				double lat = surface->GetY();

				inverseProjection(lon, lat);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->GetFeatureRelationCount() + fr->GetInformationRelationCount();

				csFrid.Format(_T("%s"), fr->GetIDAsWString().c_str());
				csFoid.Format(_T("%d"), 0);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), surface->GetType());
				csName.Format(_T("%s"), cell->GetFeatureTypeCodeByID(fr->GetID()).c_str());
				csAssoCnt.Format(_T("%d"), assoCnt);

				csa.Add(
					_T("0|||") +
					csFoid + _T("|||") +
					csFrid + _T("|||") +
					csLat + _T("|||") +
					csLon + _T("|||") +
					csType + _T("|||") +
					csName + _T("|||") +
					csAssoCnt + _T("|||") +
					featureType);
			}
		}
	}

	// Composite curve
	for (int i = 0; i < featureCount; i++) {
		auto fr = cell->GetFeatureTypeByIndex(i);
		if (fr->geometry == nullptr || fr->geometry->GetType() != SGeometryType::CompositeCurve)
		{
			continue;
		}

		SCompositeCurve* compositeCurve = (SCompositeCurve*)fr->geometry;
		if (MBR::CheckOverlap(pickMBR, fr->geometry->m_mbr))
		{
			SSurface mbrArea(&pickMBR);

			if (SGeometricFuc::overlap(compositeCurve, &mbrArea) == 1)
			{
				CString csFoid;
				CString csFrid;
				CString csLat;
				CString csLon;
				CString csType;
				CString csName;
				CString csAssoCnt;

				double lon = compositeCurve->GetX();
				double lat = compositeCurve->GetY();

				inverseProjection(lat, lon);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->GetFeatureRelationCount() + fr->GetInformationRelationCount();

				csFrid.Format(_T("%s"), fr->GetIDAsWString().c_str());
				csFoid.Format(_T("%d"), 0);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), compositeCurve->GetType());
				csName.Format(_T("%s"), cell->GetFeatureTypeCodeByID(fr->GetID()).c_str());
				csAssoCnt.Format(_T("%d"), assoCnt);

				csa.Add(
					_T("0|||") +
					csFoid + _T("|||") +
					csFrid + _T("|||") +
					csLat + _T("|||") +
					csLon + _T("|||") +
					csType + _T("|||") +
					csName + _T("|||") +
					csAssoCnt + _T("|||") +
					featureType);
			}
		}
	}

	// Curve
	for (int i = 0; i < featureCount; i++) {
		auto fr = cell->GetFeatureTypeByIndex(i);
		if (fr->geometry == nullptr || fr->geometry->GetType() != SGeometryType::Curve)
		{
			continue;
		}

		SCurve* curve = (SCurve*)fr->geometry;
		if (MBR::CheckOverlap(pickMBR, fr->geometry->m_mbr))
		{
			SSurface mbrArea(&pickMBR);

			if (SGeometricFuc::overlap(curve, &mbrArea) == 1)
			{
				CString csFoid;
				CString csFrid;
				CString csLat;
				CString csLon;
				CString csType;
				CString csName;
				CString csAssoCnt;

				double lon = curve->GetX();
				double lat = curve->GetY();

				inverseProjection(lon, lat);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->GetFeatureRelationCount() + fr->GetInformationRelationCount();

				csFrid.Format(_T("%s"), fr->GetIDAsWString().c_str());
				csFoid.Format(_T("%d"), 0);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), curve->GetType());
				csName.Format(_T("%s"), cell->GetFeatureTypeCodeByID(fr->GetID()).c_str());
				csAssoCnt.Format(_T("%d"), assoCnt);

				csa.Add(
					_T("0|||") +
					csFoid + _T("|||") +
					csFrid + _T("|||") +
					csLat + _T("|||") +
					csLon + _T("|||") +
					csType + _T("|||") +
					csName + _T("|||") +
					csAssoCnt + _T("|||") +
					featureType);
			}
		}
	}

	// Point or Multi point
	for (int i = 0; i < featureCount; i++) {
		auto fr = cell->GetFeatureTypeByIndex(i);
		if (fr->geometry == nullptr || 
			(fr->geometry->GetType() != SGeometryType::Point && fr->geometry->GetType() != SGeometryType::MultiPoint))
		{
			continue;
		}

		SGeometry* sgeo = (SGeometry*)fr->geometry;
		if (MBR::CheckOverlap(pickMBR, fr->geometry->m_mbr))
		{
			if (sgeo->GetType() == SGeometryType::MultiPoint)		// Point
			{
				auto multiPoint = (SMultiPoint*)fr->geometry;

				for (int i = 0; i < multiPoint->GetNumPoints(); i++)
				{
					double geoX = multiPoint->GetX(i);
					double geoY = multiPoint->GetY(i);

					if (pickMBR.PtInMBR(geoX, geoY))
					{
						CString csFoid, csFrid, csLat, csLon, csType, csName, csAssoCnt;

						inverseProjection(geoX, geoY);

						std::vector<int>::size_type assoCnt;
						assoCnt = fr->GetFeatureRelationCount() + fr->GetInformationRelationCount();

						csFrid.Format(_T("%s"), fr->GetIDAsWString().c_str());
						csFoid.Format(_T("%d"), 0);
						csLat.Format(_T("%f"), geoY);
						csLon.Format(_T("%f"), geoX);
						csType.Format(_T("%d"), multiPoint->GetType());
						csName.Format(_T("%s"), cell->GetFeatureTypeCodeByID(fr->GetID()).c_str());
						csAssoCnt.Format(_T("%d"), assoCnt);

						csa.Add(
							_T("0|||") +
							csFoid + _T("|||") +
							csFrid + _T("|||") +
							csLat + _T("|||") +
							csLon + _T("|||") +
							csType + _T("|||") +
							csName + _T("|||") +
							csAssoCnt + _T("|||") +
							featureType);

						break;
					}
				}
			}
			else if (sgeo->GetType() == SGeometryType::Point)
			{
				double geoX = ((SPoint*)fr->geometry)->x;
				double geoY = ((SPoint*)fr->geometry)->y;

				if (pickMBR.PtInMBR(geoX, geoY))
				{
					CString csFoid, csFrid, csLat, csLon, csType, csName, csAssoCnt;

					SPoint* sr = (SPoint*)fr->geometry;
					double lon = sr->x;
					double lat = sr->y;

					inverseProjection(lat, lon);

					std::vector<int>::size_type assoCnt;
					assoCnt = fr->GetFeatureRelationCount() + fr->GetInformationRelationCount();

					csFrid.Format(_T("%s"), fr->GetIDAsWString().c_str());
					csFoid.Format(_T("%d"), 0);
					csLat.Format(_T("%f"), lat);
					csLon.Format(_T("%f"), lon);
					csType.Format(_T("%d"), sr->GetType());
					csName.Format(_T("%s"), cell->GetFeatureTypeCodeByID(fr->GetID()).c_str());
					csAssoCnt.Format(_T("%d"), assoCnt);

					csa.Add(
						_T("0|||") +
						csFoid + _T("|||") +
						csFrid + _T("|||") +
						csLat + _T("|||") +
						csLon + _T("|||") +
						csType + _T("|||") +
						csName + _T("|||") +
						csAssoCnt + _T("|||") +
						featureType);
				}
			}
		}
	}

	for (int i = 0; i < csa.GetCount(); i++)
	{
		auto str = csa[i];
		OutputDebugString(str + L"\n");
	}

	theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, cell, L"0");
}

void COpenS100View::ESC()
{
	SetPick();

	MapRefresh(); 
}

void COpenS100View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		theApp.gisLib->MoveMap(-10, 0);
		MapRefresh();
		break;
	case VK_RIGHT:
		theApp.gisLib->MoveMap(10, 0);
		MapRefresh();
		break;
	case VK_UP:
		theApp.gisLib->MoveMap(0, -10);
		MapRefresh();
		break;
	case VK_DOWN:
		theApp.gisLib->MoveMap(0, 10);
		MapRefresh();
		break;
	case VK_ESCAPE:
		ESC();
		break;
	case VK_ADD:
		MapPlus();
		MapRefresh();
		break;
	case VK_SUBTRACT:
		MapMinus();
		MapRefresh();
		break;
	case VK_PRIOR:
		theApp.gisLib->ZoomIn(ZOOM_FACTOR);
		MapRefresh();
		break;
	case VK_NEXT:
		theApp.gisLib->ZoomOut(ZOOM_FACTOR);
		MapRefresh();
		break;
	case VK_HOME:
		theApp.gisLib->GetScaler()->Rotate(-10);
		MapRefresh();
		break;
	case VK_END:
		theApp.gisLib->GetScaler()->Rotate(10);
		MapRefresh();
		break;
	case VK_F1:
		PointFeatureList();
		break;
	case VK_F2:
		LineFeatureList();
		break;
	case VK_F3:
		AreaFeatureList();
		break;
	case VK_F4:
		//CopyLayer();
		//TestGISLibrary::TestCreateNewCode();
		theApp.gisLib->GetLayerManager()->SetS100Scale(80000);
		break;
	case VK_F5:
		TestGISLibrary::TestSave();
		break;
	case VK_F6:
		TestGISLibrary::TestSave();
		break;
	case VK_DELETE:
		DeleteSelectedFeature();
		MapRefresh();
		break;
	case 'N':
		TestGISLibrary::CreateNewLayer();
		theApp.m_pDockablePaneLayerManager.UpdateList();
		MapRefresh();
		break;
	case 'C':
		TestGISLibrary::CopySelectedFeatureToNewLayer();
		theApp.m_pDockablePaneLayerManager.UpdateList();
		MapRefresh();
		break;
	case 'O':
		TestGISLibrary::OffFeature();
		MapRefresh();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenS100View::PointFeatureList()
{
	if (theApp.gisLib)
	{
		auto fc = theApp.gisLib->GetCatalogManager()->getFC("S-101");
		std::vector<FeatureType*> features;
		fc->GetPointFeatures(features);

		OutputDebugString(L"Name, Code, Definition, Alias\n");

		for (auto i = features.begin(); i != features.end(); i++)
		{
			CString name = (*i)->GetName().c_str();
			CString code = (*i)->GetCodeAsWString().c_str();
			CString definition = (*i)->GetDefinition().c_str();
			auto alias = (*i)->GetAlias();

			CString str;
			str.Format(L"%s, %s, %s, ", name, code, definition);
			for (std::list<std::wstring>::const_iterator j = alias.begin(); j != alias.end(); j++)
			{
				if (j == alias.begin())
				{
					str.AppendFormat(L"%s", j->c_str());
				}
				else
				{
					str.AppendFormat(L";%s", j->c_str());
				}
			}

			OutputDebugString(str + L"\n");
		}
	}
}

void COpenS100View::LineFeatureList()
{
	if (theApp.gisLib)
	{
		auto fc = theApp.gisLib->GetCatalogManager()->getFC();
		std::vector<FeatureType*> features;
		fc->GetLineFeatures(features);

		OutputDebugString(L"Name, Code, Definition, Alias\n");

		for (auto i = features.begin(); i != features.end(); i++)
		{
			CString name = (*i)->GetName().c_str();
			CString code = (*i)->GetCodeAsWString().c_str();
			CString definition = (*i)->GetDefinition().c_str();
			auto alias = (*i)->GetAlias();

			CString str;
			str.Format(L"%s, %s, %s, ", name, code, definition);
			for (std::list<std::wstring>::const_iterator j = alias.begin(); j != alias.end(); j++)
			{
				if (j == alias.begin())
				{
					str.AppendFormat(L"%s", j->c_str());
				}
				else
				{
					str.AppendFormat(L";%s", j->c_str());
				}
			}

			OutputDebugString(str + L"\n");
		}
	}
}

void COpenS100View::AreaFeatureList()
{
	if (theApp.gisLib)
	{
		auto fc = theApp.gisLib->GetCatalogManager()->getFC();
		std::vector<FeatureType*> features;
		fc->GetAreaFeatures(features);

		OutputDebugString(L"Name, Code, Definition, Alias\n");

		for (auto i = features.begin(); i != features.end(); i++)
		{
			CString name = (*i)->GetName().c_str();
			CString code = (*i)->GetCodeAsWString().c_str();
			CString definition = (*i)->GetDefinition().c_str();
			auto alias = (*i)->GetAlias();

			CString str;
			str.Format(L"%s, %s, %s, ", name, code, definition);
			for (std::list<std::wstring>::const_iterator j = alias.begin(); j != alias.end(); j++)
			{
				if (j == alias.begin())
				{
					str.AppendFormat(L"%s", j->c_str());
				}
				else
				{
					str.AppendFormat(L";%s", j->c_str());
				}
			}

			OutputDebugString(str + L"\n");
		}
	}
}

void COpenS100View::CopyLayer()
{
	auto layer1 = (S100Layer*)theApp.gisLib->GetLayer(0);
	auto layer2 = (S100Layer*)theApp.gisLib->GetLayer(1);

	auto enc1 = (S101Cell*)layer1->GetSpatialObject();
	auto enc2 = (S101Cell*)layer2->GetSpatialObject();

	S101Creator creator(layer1->GetFeatureCatalog(), enc1);

	auto enc2Features = enc2->GetVecFeature();
	for (auto i = enc2Features.begin(); i != enc2Features.end(); i++)
	{
		auto feature = *i;
		auto newFeature = creator.AddFeature(std::wstring(enc2->m_dsgir.GetFeatureCode(feature->GetNumericCode())));
		if (feature->geometry)
		{
			unsigned char* wkb = nullptr;
			int sizeWKB = 0;
			if (feature->geometry->ExportToWkb(&wkb, &sizeWKB))
			{
				if (feature->geometry->GetType() == SGeometryType::Point)
				{
					creator.SetPointGeometry(newFeature, wkb, sizeWKB);
				}
			}
		}
	}

	enc1->Save(L"..\\TEMP\\edit.000");
}

void COpenS100View::DeleteSelectedFeature()
{
	auto key = theApp.m_DockablePaneCurrentSelection.pDlg->GetSelectedRecordName();
	RecordName selectedRecordName(key);
	s101Creator->DeleteFeature(selectedRecordName.RCID);
	theApp.gisLib->S101RebuildPortrayal();
	theApp.m_DockablePaneCurrentSelection.RemoveAll();
	SetPick(nullptr, nullptr);
}

void COpenS100View::SetPick(S100SpatialObject* enc, std::wstring featureID)
{
	encPick = enc;
	featurePick = featureID;
	
	if (enc && enc->GetProductNumber() == 101)
	{
		if (s101Creator == nullptr)
			s101Creator = new S101Creator((S101Cell*)enc);
		s101Creator->Set(theApp.gisLib->GetCatalogManager()->getFC(), (S101Cell*)enc);
	}
	//else
	//{
	//	s101Creator->Set(nullptr, nullptr);
	//}
}


std::wstring COpenS100View::utf8_to_wstring(const std::string& str) {
#ifdef _WIN32
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
#else
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
#endif
}

std::string COpenS100View::wstring_to_utf8(const std::wstring& wstr) {
#ifdef _WIN32
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
#else
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
#endif
}

void COpenS100View::print_utf8(const std::string& str) {
#ifdef _WIN32
	// Windows console doesn't handle UTF-8 well by default, so we convert to wide string
	std::wcout << utf8_to_wstring(str);
#else
	// On Unix-like systems, UTF-8 output is usually well supported
	std::cout << str;
#endif
}

void COpenS100View::debug_print_utf8(const std::string& str) {
#ifdef _WIN32
	// Windows console doesn't handle UTF-8 well by default, so we convert to wide string
	OutputDebugStringW(utf8_to_wstring(str).c_str());
#else
	// On Unix-like systems, UTF-8 output is usually well supported
	std::cerr << str;
#endif
}

//std::wstring COpenS100View::utf8_to_wstring(const std::string& str) {
//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//	return converter.from_bytes(str);
//}
//
//std::string COpenS100View::wstring_to_utf8(const std::wstring& wstr) {
//	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//	return converter.to_bytes(wstr);
//}
//
//void COpenS100View::print_utf8(const std::string& str) {
//#ifdef _WIN32
//	// Windows console doesn't handle UTF-8 well by default, so we convert to wide string
//	std::wcout << utf8_to_wstring(str);
//#else
//	// On Unix-like systems, UTF-8 output is usually well supported
//	std::cout << str;
//#endif
//}


void COpenS100View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CString filepath = DataSetManagerSupport::GetInstance().m_FolderPath + DataSetManagerSupport::GetInstance().m_FileName;
	theApp.gisLib->AddLayer(filepath);
	

	/*for (auto item : DataSetManagerSupport::GetInstance().initDataFileList())
		theApp.gisLib->AddLayer(LibMFCUtil::StringToWString(item).c_str());*/

	theApp.m_pDockablePaneLayerManager.UpdateList();


	TranslationPackageType* translationPackage = new TranslationPackageType;
	if (translationPackage->LoadFromFile("..\\pizza_fc_de.xml"))
	{
		debug_print_utf8("Language: " + translationPackage->Language + "\n");
		debug_print_utf8("Issue Date: " + translationPackage->IssueDate + "\n");
		debug_print_utf8("Issue Time: " + translationPackage->IssueTime + "\n");
		/*print_utf8("Responsible Party: " + translationPackage->ResponsibleParty + "\n");*/

		for (const auto& sourceFile : translationPackage->SourceFiles) {
			debug_print_utf8("Source File Identifier: " + sourceFile.Header.ResourceIdentifier + "\n");
			for (const auto& id : sourceFile.Header.Identifications) {
				debug_print_utf8("  Path: " + id.Path + ", Value: " + id.Value + "\n");
			}
			for (const auto& item : sourceFile.TranslationItems) {
				debug_print_utf8("  Translation Item Path: " + item.Path + "\n");
				debug_print_utf8("    Original: " + item.Original + "\n");
				debug_print_utf8("    Status: " + item.ItemStatus.toString() + "\n");
				debug_print_utf8("    Translation: " + item.Translation + "\n");
			}
		}
	}
	else {
		debug_print_utf8("Failed to load language pack.\n");
	}

}
