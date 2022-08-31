#include "pch.h"
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
#include "CDialogExFeatureInformationList.h"
#include "TestGISLibrary.h"

#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/Layer.h"
#include "../GISLibrary/R_FeatureRecord.h"
#include "../GISLibrary/CodeWithNumericCode.h"
#include "../GISLibrary/S101Creator.h"
#include "../GISLibrary/S101Layer.h"
#include "../GISLibrary/S101Cell.h"

#include "../GeoMetryLibrary/GeometricFuc.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/GeoPolyline.h"

#include "../S100Geometry/SPoint.h"
#include "../S100Geometry/SMultiPoint.h"
#include "../S100Geometry/SCompositeCurve.h"
#include "../S100Geometry/SSurface.h"
#include "../S100Geometry/SGeometricFuc.h"

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

#include "../LatLonUtility/LatLonUtility.h"
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
	ON_COMMAND(T10, &COpenS100App::OnAppAbout)
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

COpenS100View::COpenS100View() 
{
	theApp.pView = this;
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

	CRect rect;
	GetClientRect(&rect);
	theApp.gisLib->SetViewMBR(rect);

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
			DrawFromMapRefresh(&map_dc, rect);

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
	theApp.gisLib->ZoomOut(0, viewRect.Width() / 2, viewRect.Height() / 2);
	theApp.gisLib->UpdateScale();

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
	ClientToScreen(&point);
	OnContextMenu(this, point);
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
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_FILEMUSTEXIST, _T("ENC Files (*.000)|*.000"), this);

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();

		//RemoveLoadFile(); //Delete the existing history.
		theApp.gisLib->AddLayer(filePath); //Add a layer.
		theApp.m_pDockablePaneLayerManager.UpdateList();
		MapRefresh();
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
	MapRefresh();
}

void COpenS100View::MapMinus()
{
	CRect rect;
	GetClientRect(rect);
	theApp.gisLib->ZoomOut(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
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

	lm->GetScaler()->SetMap(layerMBR);
	theApp.MapRefresh();
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

	S101Cell* cell = (S101Cell*)layer->m_spatialObject;

	if (nullptr == dialogInformationType)
	{
		dialogInformationType = new CDialogViewInformationType(this);
		dialogInformationType->Create(IDD_DIALOG_INFORMATIONTYPE);
	}

	dialogInformationType->SetInformationFeatureList(cell);
	dialogInformationType->ShowWindow(SW_SHOW);
}

void COpenS100View::Setting()
{
	CConfigrationDlg dlg(this);

	auto fc = theApp.gisLib->GetFC();
	if (fc)
	{
		dlg.InitS101FeatureTypes(fc);
	}

	if (m_systemFontList.size() == 0)
	{
		// <FONT LIST>
		HRESULT hr;
		IDWriteFactory* pDWriteFactory = theApp.gisLib->D2.pDWriteFactory;
		IDWriteFontCollection* pFontCollection = NULL;

		// Get the system font collection.
		//if (SUCCEEDED(hr))
		{
			hr = pDWriteFactory->GetSystemFontCollection(&pFontCollection);
		}
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

						delete name;
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


void COpenS100View::MapRefresh()
{
	m_bMapRefesh = true;
	Invalidate(FALSE);
}

void COpenS100View::OpenWorldMap()
{
	theApp.gisLib->AddBackgroundLayer(_T("../ProgramData/World/World.shp"));

	MapRefresh();
}

int COpenS100View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	FeatureCatalogue* fc = new FeatureCatalogue(L"..\\ProgramData\\xml\\S-101_FC.xml");
	PortrayalCatalogue* pc = new PortrayalCatalogue(L"..\\ProgramData\\S101_Portrayal\\portrayal_catalogue.xml");

	theApp.gisLib->InitLibrary(fc, pc);

	//gisLib->InitLibrary(L"../ProgramData/xml/S-101_FC.xml", L"../ProgramData/S101_Portrayal/portrayal_catalogue.xml");

	//PointFeatureList();
	//LineFeatureList();
	//AreaFeatureList();

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
	CView::OnMButtonUp(nFlags, point);
}


void COpenS100View::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	CRect cr;
	GetClientRect(&cr);
	theApp.gisLib->DeviceToWorld(cr.Width() / 2, cr.Height() / 2, &moveMX, &moveMY);

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

	if (theApp.gisLib->GetScaler()->GetRotationDegree())
	{
		double radian = (180 + theApp.gisLib->GetScaler()->GetRotationDegree()) * DEG2RAD;

		FLOAT tempX = (float)dy * (float)sin(radian) + (float)dx * (float)cos(radian);
		FLOAT tempY = (float)dy * (float)cos(radian) - (float)dx * (float)sin(radian);

		dx = (int)-tempX;
		dy = (int)-tempY;

	}

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
			frPick = nullptr;
			PickReport(calc_point);
		}
	}

	if (isMoved == true)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		theApp.gisLib->MoveMap(cr.Width() / 2 + dx, cr.Height() / 2 + dy, moveMX, moveMY);
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
	m_strFormatedLongitude = strLat;


	
	
	CString strFomatedInfo = _T("");
	strFomatedInfo.Format(_T("%s , %s , %s"), m_strFormatedScale, m_strFormatedLongitude, m_strFormatedLongitude);

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

	theApp.m_pDockablePaneLayerManager.UpdateList();

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
	}
	// When you lower the mouse wheel => Zoom out.
	else
	{
		theApp.gisLib->ZoomOut(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
	}

	MapRefresh();


	m_strFormatedScale = theApp.gisLib->GetScaler()->GetFormatedScale();
	CString strFomatedInfo = _T("");
	strFomatedInfo.Format(_T("%s , %s , %s"), m_strFormatedScale, m_strFormatedLongitude, m_strFormatedLongitude);

	CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
	frame->SetMessageText(strFomatedInfo);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void COpenS100View::DrawPickReport(HDC& _hdc, int offsetX, int offsetY)
{
	Graphics gPick(_hdc);
	if (frPick != nullptr)
	{
		DrawS101PickReport(gPick, offsetX, offsetY);
	}
}

void COpenS100View::DrawS101PickReport(Graphics& g, int offsetX, int offsetY)
{
	// Point
	if (frPick->m_geometry->IsPoint())
	{
		long x = 0;
		long y = 0;

		theApp.gisLib->WorldToDevice(((SPoint*)(frPick->m_geometry))->x, ((SPoint*)(frPick->m_geometry))->y, &x, &y);
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
	else if (frPick->m_geometry->IsMultiPoint())
	{
		auto multiPoint = (SMultiPoint*)frPick->m_geometry;

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
	else if (frPick->m_geometry->IsCurve())
	{
		SolidBrush brush(Color(255, 0, 0));

		if (frPick->m_geometry->type == 2)
		{
			SCompositeCurve* cc = (SCompositeCurve*)(frPick->m_geometry);

			for (auto it = cc->m_listCurveLink.begin(); it != cc->m_listCurveLink.end(); it++)
			{
				//SCurve* c = (*it).GetCurve();
				SCurve* c = (*it);
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
		}
	}
	else if (5 == frPick->m_geometry->type)
	{
		SolidBrush brush(Color(255, 0, 0));

		SCurve* c = (SCurve*)(frPick->m_geometry);

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
	else if (frPick->m_geometry->IsSurface())
	{
		auto surface = (SSurface*)frPick->m_geometry;
		auto geometry = surface->GetNewD2Geometry(theApp.gisLib->D2.pD2Factory, theApp.gisLib->GetScaler());

		D2D1_COLOR_F color{};
		color.r = 255 / float(255.0);
		color.g = 50 / float(255.0);
		color.b = 50 / float(255.0);
		color.a = float(0.7);

		HDC hdc = g.GetHDC();
		theApp.gisLib->D2.Begin(hdc, theApp.gisLib->GetScaler()->GetScreenRect());
		if (geometry)
		{
			theApp.gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(float(offsetX), float(offsetY)));
			theApp.gisLib->D2.pBrush->SetColor(color);
			theApp.gisLib->D2.pBrush->SetOpacity(float(0.7));
			theApp.gisLib->D2.pRT->FillGeometry(geometry, theApp.gisLib->D2.pBrush);
			SafeRelease(&geometry);
		}
		theApp.gisLib->D2.End();
	}
}

void COpenS100View::ClearPickReport()
{
	frPick = nullptr;
}

void COpenS100View::PickReport(CPoint _point)
{
	auto selectedLayerIndex = theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedLayerIndex();

	if (selectedLayerIndex < 0 || selectedLayerIndex >> theApp.gisLib->GetLayerManager()->LayerCount())
	{
		return;
	}

	PickReport(_point, selectedLayerIndex);
}

void COpenS100View::PickReport(CPoint _point, int layerIndex)
{
	auto layer = theApp.gisLib->GetLayerManager()->GetLayer(layerIndex);
	if (nullptr == layer)
	{
		return;
	}

	auto cell = (S101Cell*)layer->GetSpatialObject();
	if (nullptr == cell)
	{
		return;
	}

	theApp.gisLib->DeviceToWorld(_point.x, _point.y, &ptPickX, &ptPickY);
	ptPick = _point;

	CString featureType = L"Feature";
	CStringArray csa;

	double xmin = 0;
	double ymin = 0;
	double xmax = 0;
	double ymax = 0;

	LONG spt_x = m_ptMDown.x;
	LONG spt_y = m_ptMDown.y;

	LONG ept_x = m_ptMUp.x;
	LONG ept_y = m_ptMUp.y;

	// point,
	if (m_ptMUp.x == 0 || m_sp == m_ep)
	{
		theApp.gisLib->DeviceToWorld(_point.x - 5, _point.y + 5, &xmin, &ymin);  // start point
		theApp.gisLib->DeviceToWorld(_point.x + 5, _point.y - 5, &xmax, &ymax);  // end point
	}
	// square
	else
	{
		if (spt_x < ept_x && spt_y > ept_y)
		{
			theApp.gisLib->DeviceToWorld(spt_x, ept_y, &xmin, &ymax);
			theApp.gisLib->DeviceToWorld(ept_x, spt_y, &xmax, &ymin);
		}
		else if (spt_x < ept_x && spt_y < ept_y)
		{
			theApp.gisLib->DeviceToWorld(spt_x, spt_y, &xmin, &ymax);
			theApp.gisLib->DeviceToWorld(ept_x, ept_y, &xmax, &ymin);
		}

		else if (spt_x > ept_x && spt_y > ept_y)
		{
			theApp.gisLib->DeviceToWorld(ept_x, ept_y, &xmin, &ymax);
			theApp.gisLib->DeviceToWorld(spt_x, spt_y, &xmax, &ymin);
		}

		else if (spt_x > ept_x && spt_y < ept_y)
		{
			theApp.gisLib->DeviceToWorld(ept_x, spt_y, &xmin, &ymax);
			theApp.gisLib->DeviceToWorld(spt_x, ept_y, &xmax, &ymin);
		}
	}

	MBR pickMBR(xmin, ymin, xmax, ymax);

	__int64 key = 0;
	R_FeatureRecord* fr = nullptr;

	POSITION pos = cell->GetFeatureStartPosition();

	// Area Type search
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, fr);
		if (fr->m_geometry == nullptr || fr->m_geometry->type != 3)
		{
			continue;
		}

		SSurface* surface = (SSurface*)fr->m_geometry;

		if (MBR::CheckOverlap(pickMBR, fr->m_geometry->m_mbr))
		{
			int code = fr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);

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

				double lon = surface->m_pPoints[0].x;
				double lat = surface->m_pPoints[0].y;

				inverseProjection(lon, lat);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->m_fasc.size() + fr->m_inas.size();

				csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
				csFoid.Format(_T("%d"), fr->m_foid.FIDN);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), surface->type);
				csName.Format(_T("%s"), itor->second->m_code);
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
	pos = cell->GetFeatureStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, fr);
		if (fr->m_geometry == nullptr || fr->m_geometry->type != 2)
		{
			continue;
		}

		SCompositeCurve* compositeCurve = (SCompositeCurve*)fr->m_geometry;
		if (MBR::CheckOverlap(pickMBR, fr->m_geometry->m_mbr))
		{
			int code = fr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);

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

				double lat = 0;
				double lon = 0;
				if (compositeCurve->m_listCurveLink.size() > 0)
				{
					lon = (*compositeCurve->m_listCurveLink.begin())->m_pPoints[0].x;
					lat = (*compositeCurve->m_listCurveLink.begin())->m_pPoints[0].y;
				}

				inverseProjection(lat, lon);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

				csFoid.Format(_T("%d"), fr->m_foid.FIDN);
				csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), compositeCurve->type);
				csName.Format(_T("%s"), itor->second->m_code);
				csAssoCnt.Format(_T("%d"), assoCnt);
				csa.Add(_T("0|||") + csFoid + _T("|||") + csFrid + _T("|||") + csLat + _T("|||") + csLon + _T("|||") + csType + _T("|||") + csName + _T("|||") + csAssoCnt + _T("|||") + featureType);
			}
		}
	}

	// Curve
	pos = cell->GetFeatureStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, fr);
		if (fr->m_geometry == nullptr || fr->m_geometry->type != 5)
		{
			continue;
		}

		SCurve* curve = (SCurve*)fr->m_geometry;
		if (MBR::CheckOverlap(pickMBR, fr->m_geometry->m_mbr))
		{
			int code = fr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);

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

				double lat = curve->GetY(0);
				double lon = curve->GetX(0);

				inverseProjection(lon, lat);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

				csFoid.Format(_T("%d"), fr->m_foid.FIDN);
				csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
				csLat.Format(_T("%f"), lat);
				csLon.Format(_T("%f"), lon);
				csType.Format(_T("%d"), curve->type);
				csName.Format(_T("%s"), itor->second->m_code);
				csAssoCnt.Format(_T("%d"), assoCnt);
				csa.Add(_T("0|||") + csFoid + _T("|||") + csFrid + _T("|||") + csLat + _T("|||") + csLon + _T("|||") + csType + _T("|||") + csName + _T("|||") + csAssoCnt + _T("|||") + featureType);
			}
		}
	}

	// Point or Multi point
	pos = cell->GetFeatureStartPosition();
	while (pos != NULL)
	{
		__int64 key = 0;
		R_FeatureRecord* fr = NULL;
		cell->GetNextAssoc(pos, key, fr);
		if (fr->m_geometry == nullptr || (fr->m_geometry->type != 1 && fr->m_geometry->type != 4))
		{
			continue;
		}

		SGeometry* sgeo = (SGeometry*)fr->m_geometry;
		if (MBR::CheckOverlap(pickMBR, fr->m_geometry->m_mbr))
		{
			int code = fr->m_frid.m_nftc;

			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);
			if (sgeo->IsMultiPoint())		// Point
			{
				auto multiPoint = (SMultiPoint*)fr->m_geometry;

				for (int i = 0; i < multiPoint->GetNumPoints(); i++)
				{
					double geoX = multiPoint->GetX(i);
					double geoY = multiPoint->GetY(i);

					if (pickMBR.PtInMBR(geoX, geoY))
					{
						CString csFoid, csFrid, csLat, csLon, csType, csName, csAssoCnt;

						inverseProjection(geoX, geoY);

						std::vector<int>::size_type assoCnt;
						assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

						csFoid.Format(_T("%d"), fr->m_foid.FIDN);
						csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
						csLat.Format(_T("%f"), geoY);
						csLon.Format(_T("%f"), geoX);
						csType.Format(_T("%d"), multiPoint->GetType());
						csName.Format(_T("%s"), itor->second->m_code);
						csAssoCnt.Format(_T("%d"), assoCnt);
						csa.Add(_T("0|||") + csFoid + _T("|||") + csFrid + _T("|||") + csLat + _T("|||") + csLon + _T("|||") + csType + _T("|||") + csName + _T("|||") + csAssoCnt + _T("|||") + featureType);

						break;
					}
				}
			}
			else if (sgeo->IsPoint())
			{
				double geoX = ((SPoint*)fr->m_geometry)->x;
				double geoY = ((SPoint*)fr->m_geometry)->y;

				if (((pickMBR.xmin <= geoX) && (geoX <= pickMBR.xmax))
					&& ((pickMBR.ymin <= geoY) && (geoY <= pickMBR.ymax)))
				{
					CString csFoid, csFrid, csLat, csLon, csType, csName, csAssoCnt;

					SPoint* sr = (SPoint*)fr->m_geometry;
					double lon = sr->x;
					double lat = sr->y;

					inverseProjection(lat, lon);

					std::vector<int>::size_type assoCnt;
					assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

					csFoid.Format(_T("%d"), fr->m_foid.FIDN);
					csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
					csLat.Format(_T("%f"), lat);
					csLon.Format(_T("%f"), lon);
					csType.Format(_T("%d"), sr->type);
					csName.Format(_T("%s"), itor->second->m_code);
					csAssoCnt.Format(_T("%d"), assoCnt);
					csa.Add(_T("0|||") + csFoid + _T("|||") + csFrid + _T("|||") + csLat + _T("|||") + csLon + _T("|||") + csType + _T("|||") + csName + _T("|||") + csAssoCnt + _T("|||") + featureType);
				}
			}
		}
	}

	theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, cell, L"0");
}

void COpenS100View::SetPickReportFeature(R_FeatureRecord* _fr)
{
	frPick = _fr;
	Layer* l = nullptr;

	l = (Layer*)theApp.gisLib->GetLayer(0);

	if (l == NULL)
	{
		CString str;
		str.Format(_T("Layer (%d) could not be retrieved."), 0);
		AfxMessageBox(str);
		return;
	}

	// Output WKB string
	//if (frPick->m_geometry)
	//{
	//	unsigned char* wkb = nullptr;
	//	int size = 0;
	//	frPick->m_geometry->ExportToWkb(&wkb, &size);

	//	CString str;
	//	for (int i = 0; i < size; i++)
	//	{
	//		str.AppendFormat(_T("%02X"), wkb[i] & 0xff);
	//	}

	//	LibMFCUtil::OutputDebugLongString(L"\n" + str + L"\n");

	//	delete[] wkb;

	//	//if (frPick->m_geometry->type == 1)
	//	//if (frPick->m_geometry->type == 5)
	//	//if (frPick->m_geometry->type == 2)
	//	//if (frPick->m_geometry->type == 3)
	//	if (frPick->m_geometry->type == 4)
	//	{
	//		//std::string hex = "0103000000010000003f000000ed5003bcbb7b4e40be672442234340c0ed5003bcbb7b4e40be672442234340c0154e7743bf7b4e40b2570ee3244340c052ba9976d67b4e40c03456ac2b4340c0210038f6ec7b4e4089d3a46f2d4340c0c83c3c951d7c4e400074982f2f4340c04fec46c4397c4e404bff379b334340c02c2e8eca4d7c4e4060cd0182394340c0a30f4c24877c4e40b61d09ea4a4340c026d7039c947c4e40d1dc54ee4f4340c069d608b3bf7c4e40f03504c7654340c07333dc80cf7c4e405e4fcf166b4340c0e75a0fbadf7c4e409ed6c8096e4340c057444df4f97c4e4044e7ebe86e4340c0fb6e5fac147d4e400795b88e714340c08f119a6e237d4e40b35cdb80744340c0beccc17d2e7d4e400990fc1c7a4340c07f8a8807397d4e4006781c50804340c0b28c1e09457d4e40131be20e8a4340c00fdcdcf3577d4e40d906ee409d4340c096abc4e1717d4e40e391d332ad4340c0998ea5b4a37d4e409df7ff71c24340c045c99a47b47d4e401bcc704dcb4340c0eb80da5ec17d4e40ce2335a3d54340c06dc19ceada7d4e4053aae91fe94340c07c4b395fec7d4e40454f255ef94340c02aedc330167e4e40b44f11f1204440c0729cca58237e4e40857e01182a4440c08c6a11514c7e4e406272593b404440c06af1ce46537e4e4013affc43444440c06af1ce46537e4e4013affc43444440c06af1ce46537e4e4013affc43444440c06af1ce46537e4e4013affc43444440c049bce1e3b87d4e4013affc43444440c049bce1e3b87d4e4013affc43444440c049bce1e3b87d4e4013affc43444440c049bce1e3b87d4e4013affc43444440c0bda4d6a0d47c4e4013affc43444440c0bda4d6a0d47c4e4013affc43444440c0bda4d6a0d47c4e4013affc43444440c0bda4d6a0d47c4e4013affc43444440c08a66af88ab7c4e40d698c6e52e4440c0387c77d0917c4e400690ebf0214440c075c2f0b6777c4e4058930266194440c022ac21cc487c4e40ddc2047f094440c0bcd3f8e0107c4e40218dafe2f94340c02d499eebfb7b4e40c40ce8e0f44340c043b8b87cda7b4e40ceb92583ed4340c0d81b21a8bf7b4e40e18cabdbea4340c0ed5003bcbb7b4e40ff774485ea4340c0ed5003bcbb7b4e40ff774485ea4340c0ed5003bcbb7b4e40ff774485ea4340c0ed5003bcbb7b4e40ff774485ea4340c0ed5003bcbb7b4e403f5b07077b4340c0ed5003bcbb7b4e403f5b07077b4340c0ed5003bcbb7b4e403f5b07077b4340c0ed5003bcbb7b4e403f5b07077b4340c0ed5003bcbb7b4e40de150b8d714340c0ed5003bcbb7b4e40de150b8d714340c0ed5003bcbb7b4e40de150b8d714340c0ed5003bcbb7b4e40de150b8d714340c0ed5003bcbb7b4e40be672442234340c0ed5003bcbb7b4e40be672442234340c0";
	//		//std::string hex = "010200000030000000ed5003bcbb7b4e40f8e92e3f263f40c0ed5003bcbb7b4e40f8e92e3f263f40c0f89c60a4bc7b4e40b16b7bbb253f40c021df94a8cd7b4e4054a3fc49213f40c0fe710c12ec7b4e40e5417a8a1c3f40c0440aaf6e067c4e401abc5411133f40c0979bb9d1117c4e402ff76e980d3f40c0979bb9d1117c4e402ff76e980d3f40c0979bb9d1117c4e402ff76e980d3f40c0979bb9d1117c4e402ff76e980d3f40c0e46025d52d7c4e40ad2fb720003f40c046dcad765e7c4e403496fac3e03e40c027f61acc707c4e4005fbaf73d33e40c07f051f39887c4e40de8a694bc23e40c02670eb6e9e7c4e40130544d2b83e40c02b33a5f5b77c4e40df1797aab43e40c07205ca5ccf7c4e4051a5660fb43e40c018e027b3e67c4e400d62b197b73e40c03dab1622f17c4e40350301d7ba3e40c0412e162b0f7d4e40c7061c53c13e40c07e9bb45f2d7d4e40ba803c60c33e40c01f9441ff4e7d4e40028f5ec3c23e40c0b8697ddb5d7d4e4006a2821ec53e40c08de1a0736d7d4e40581ea4a7c83e40c0b5b97bca7b7d4e406dec6d8ece3e40c0cd55f31c917d4e40e5cf5c4cd83e40c039a2d68fa87d4e40a2f14410e73e40c0217af76cc27d4e401b745314f93e40c0e997d244e97d4e40aac42bff103f40c0fd2fd7a2057e4e40da5141ea1b3f40c0837291312d7e4e407e840607203f40c0f8aa3a504c7e4e407bc84a82263f40c0272724e3627e4e4083b4102e2e3f40c00053509b827e4e40fab9a1293b3f40c0df228b8faa7e4e40cde3d5de4c3f40c0af4e84c3c17e4e40460147a7533f40c0bd9f1a2fdd7e4e40f6db8df4583f40c0f8d9b749fb7e4e40736d03d2593f40c01b749da1137f4e406721a6e9583f40c03386de87287f4e409c001eac5a3f40c08c086b08337f4e40e27899bc5c3f40c0bd1fb75f3e7f4e40b7bf69b05f3f40c0469feaeb547f4e40a7c585a8673f40c0e0e8746a787f4e405ef6eb4e773f40c0bfdd488f957f4e403fe08101843f40c0dd19129c9f7f4e40bbdb508c873f40c0a02b007ca67f4e401f2bf86d883f40c0a02b007ca67f4e401f2bf86d883f40c0";
	//		//std::string hex = "01010000004ef0f219f57d4e4027c8be1a564140c0";
	//		std::string hex = "01EC0300009D00000001E90300000BD12170247C4E4077A79608AF3E40C0E17A14AE4701384001E9030000E9BA4B3D667D4E409A057F64DF3A40C07B14AE47E11A304001E90300007B22E3ACE37D4E40DCA6D315C73A40C0AE47E17A14CE334001E9030000F381D371EB7C4E40CFD7D1DD1A3B40C0295C8FC2F5682B4001E9030000255289A18B7E4E40D8B6CDF9CE3A40C0713D0AD7A300404001E90300003FF3DEBD487D4E4065D30094753A40C052B81E85EBD11A4001E9030000E0C0E446917C4E40CDB85FF4CB3E40C05C8FC2F5289C214001E9030000CEC87B30847D4E40577325A71B3F40C0B81E85EB5138114001E903000003942B61127D4E409D121093703B40C0E17A14AE4781314001E90300005E5E6F511C7C4E40A5C97B8B3D3B40C08FC2F5285CCF244001E9030000872FB88BD57C4E4012EEDF06C63C40C048E17A14AE67304001E9030000787C7BD7A07D4E401CFF4F0F1B3D40C085EB51B81E85D33F01E9030000F4188AE0247C4E40DDAE3CED953B40C014AE47E17A34324001E90300004BF48D43587D4E40B325AB22DC3C40C0C3F5285C8F022A4001E9030000FB36B34C757C4E4043797A00E63B40C07B14AE47E19A344001E903000008D9C3A8B57D4E40D4FB7C4A843C40C0E17A14AE4701384001E903000038F878D78F7E4E409BED6536233C40C0713D0AD7A300414001E903000047FC2F7CE97C4E4044882B67EF3C40C05C8FC2F5289C214001E9030000BAD7EEC3E67C4E40068EBA8C513C40C0E17A14AE4701354001E903000083CDEFEA0B7D4E409570D701C63B40C0E17A14AE4701384001E9030000CD199B67367F4E40DA5DFB5D333D40C0E17A14AE47013F4001E9030000BDE776E5697F4E40D0C254E9DD3B40C0713D0AD7A380454001E90300009AAF928FDD7C4E40E921BF23CF4240C00AD7A3703D0A084001E903000073ED0BE8857D4E403ACD02ED0E4140C0E17A14AE47E1FCBF01E9030000C29437763F7F4E4059A48977804340C085EB51B81E05104001E90300006B9C4D47007E4E409BFF571D394340C052B81E85EBD1104001E903000043FEE3CE967F4E40BDBDB55DB24340C0E17A14AE47E1BA3F01E9030000A52C8DA9057C4E401E705D31234240C03D0AD7A3703DCABF01E9030000A4D93C0E837E4E40C8C969F40F4340C0D7A3703D0AD70C4001E903000041D24C9C817D4E4092C3712A634240C0EC51B81E85EBD9BF01E903000018E53EDE507E4E404CDE0033DF4340C085EB51B81E051C4001E9030000BADE910C947F4E408A6BC6B3154340C03D0AD7A3703DCA3F01E903000096CCFB49C67D4E4063693B01A84240C05C8FC2F5285CE33F01E90300004735913E527E4E40D7A4DB12B94240C048E17A14AE47F33F01E9030000C727092EFB7C4E4060C2B2E3D04340C0295C8FC2F5682B4001E9030000B587180A337E4E40068195438B4340C0EC51B81E856B164001E903000044C93F8E417D4E40F340BF4A994240C085EB51B81E85D33F01E903000061AFC10CD77B4E405A5DF34F154340C0F6285C8FC235214001E9030000A1FF1EBC767D4E4051572D9F9B4340C05C8FC2F5289C214001E9030000E3F7DB43677C4E4074B515FBCB3D40C0C3F5285C8F02244001E90300009FD32CD0EE7D4E4079420AF9FB3D40C05C8FC2F5289C204001E903000018601F9DBA7C4E4025C1655FC33D40C0F6285C8FC2F5EC3F01E90300009B3D2BC47E7C4E40CBC4083C8B3D40C0F6285C8FC2F5ECBF01E90300009E685721E57D4E4079814875F03C40C0F6285C8FC2352D4001E9030000C3E3912EEC7B4E4003D770EC344440C0E17A14AE4781354001E90300009658BED3097E4E40D588AAA9763A40C0E17A14AE4781324001E9030000FF36D5A4257F4E402AC4C837803F40C0B81E85EB51381D4001E903000084317D0ADB7E4E40255EF987884040C0EC51B81E85EBD93F01E90300007D9D2F51187F4E40E29A96B3D23F40C01F85EB51B89E134001E9030000DC4E1196677E4E40595249F8394040C048E17A14AE47F33F01E90300000CD462F0307E4E40D74FFF59F33F40C0E17A14AE47E1FC3F01E9030000291C412AC57E4E407912C7702B3F40C0C3F5285C8F022C4001E90300004E5BC8C8FE7D4E40EC2D8AC33E3F40C0F6285C8FC235224001E9030000B7B24467997E4E4012EC095C683F40C08FC2F5285CCF204001E90300008D9A54D91C7F4E40DA95F14AED4040C03D0AD7A3703DCABF01E903000072FB8ADAA27E4E402F02BE36764340C0EC51B81E856B124001E9030000C1278C1CD87C4E4057D0B4C4CA3A40C0D7A3703D0AD70C4001E9030000BAADE3AE147C4E40D5C276418E3A40C01F85EB51B89E154001E903000043588D25AC7D4E408C135FED284040C048E17A14AE47F3BF01E9030000C20C7CFB297E4E40E8C072840C3B40C0E17A14AE47013C4001E9030000ED7CE47BEB7D4E4020DE29D31E4240C0C3F5285C8FC2E9BF01E9030000404459AE6D7F4E40928B7B3DA94240C085EB51B81E85D3BF01E9030000458545A0557F4E40D97A8670CC4140C07B14AE47E17AF6BF01E9030000251D2FEE507C4E40F42675B80C3F40C052B81E85EBD1104001E9030000773870BD5C7D4E40B2497EC4AF3F40C0D7A3703D0AD700C001E9030000781C5080727F4E40DE3CD521374240C0C3F5285C8FC2E9BF01E90300005ED3CD0F6D7D4E407FA6B9707B3F40C085EB51B81E85D3BF01E9030000CFD4905D0E7F4E40B863A021D23D40C0E17A14AE47013A4001E9030000E20D1FC7C57D4E40CCD9965C203D40C085EB51B81E051E4001E9030000CFA78E554A7C4E40350A4966F53A40C05C8FC2F5289C214001E9030000F8B9EBB6557D4E40E9D907B4CF3E40C08FC2F5285CCF224001E90300009A3E3BE0BA7D4E408E6A227DA43F40C07B14AE47E17AF63F01E90300007A9DC36B4D7E4E40F78F2A2F9E4140C0AE47E17A14AEF9BF01E903000089DF032AD27B4E40C8F20934333F40C00AD7A3703D0A0C4001E9030000CCA3D1D3367C4E406EBB75A3343F40C0F6285C8FC2F5EC3F01E903000093CAB9B99D7E4E4058ED5003BC3F40C052B81E85EBD1104001E903000099AF8163857F4E40BF4868CBB94040C05C8FC2F5285CE33F01E90300006DD800B6CD7E4E40E40A39002C4240C0F6285C8FC2F5ECBF01E903000052550D67237E4E40E6DBCC32D54040C0F6285C8FC2F5ECBF01E9030000648EE55DF57E4E40026D61CC4C4140C0C3F5285C8FC2E9BF01E9030000C3EAD9BD267E4E4045AFF5FBB43F40C0A4703D0AD7A30D4001E9030000D9BE36D19D7E4E40C451A85D024040C03D0AD7A3703D034001E90300002AD25856F57D4E4000FA2246743F40C01F85EB51B89E114001E9030000AC0727FD187D4E40806E1E3D343F40C052B81E85EBD110C001E9030000EEB204BE477E4E4066B2C92B213E40C01F85EB51B89E1D4001E90300001E4BC4002E7F4E4083989537D13E40C0E17A14AE4701364001E90300007A4F8AFB347F4E40847DE0CF953C40C0713D0AD7A380404001E903000090604FE0427D4E406F8214E1CB3D40C0D7A3703D0AD70C4001E9030000E1DFFB766D7E4E4076F4AE30333D40C014AE47E17A34324001E90300008EC5DBEFF57B4E40F2D654713E3C40C014AE47E17A34344001E9030000CA9EF3098F7C4E40B768B74C3C3D40C01F85EB51B89E154001E9030000B629C35D627F4E40B4441D0C2B3F40C014AE47E17A34304001E90300008F386403E97B4E407AEC1D24FA3E40C07B14AE47E11A314001E903000030E53224DD7C4E409CBF0985083F40C0EC51B81E85EBD9BF01E9030000EFF2BE3B437C4E40E2C56757C64240C0B81E85EB5138114001E9030000D6851F9C4F7C4E40550D67237C4240C0EC51B81E85EBD93F01E90300005981C6F1E87B4E4020065F3D934240C0AE47E17A14AEF93F01E9030000DAF1BAD97A7D4E400541367F024340C0A4703D0AD7A30D4001E9030000BE0C6BCFD17D4E4015E17030E54240C00AD7A3703D0A004001E9030000648FF579317F4E405E7DE13A6B3E40C0E17A14AE4701314001E90300009B6846F58C7F4E4089B14CBF443E40C0E17A14AE4701374001E9030000E3FF33EFDD7D4E407D4166C28A3E40C0E17A14AE4781354001E90300003AC0DDA3487C4E40942243D83A3E40C0E17A14AE4701354001E903000044ADC497E47D4E408429CAA5F13B40C0E17A14AE47013A4001E9030000A1BAB9F8DB7C4E402FB1E88CDE3D40C085EB51B81E05124001E9030000A8B805A62E7D4E406A6803B0013E40C085EB51B81E051A4001E9030000F2FE89DBB27D4E40EA5DBC1FB73E40C014AE47E17A34324001E9030000E47A8093127C4E40C181DAB97A3D40C0C3F5285C8F022C4001E9030000458CE8E8027F4E40EBE40CC51D4340C014AE47E17A14F83F01E903000062B312A91F7D4E4074B4AA251D4340C052B81E85EBD1144001E9030000E308089C5B7C4E404EFDCDD5A03C40C0E17A14AE4701344001E90300004E3052DE7D7C4E40B1E31AFA823A40C0713D0AD7A370024001E9030000E487A5DCD87B4E4082DF2B5F3C3E40C0295C8FC2F568284001E903000046C6B4801A7E4E40BE384C8F013F40C0295C8FC2F568244001E9030000AB037BA75D7C4E40CF3A996EC83E40C014AE47E17AB4314001E9030000EA65CA3D137E4E4000D6F4EA2D3D40C05C8FC2F5289C264001E903000088719241497E4E40D55DD905833D40C0295C8FC2F568284001E903000000E1E8CF237C4E4007A74FBC143D40C07B14AE47E11A324001E903000025620097B67D4E4070C09DFAF63D40C0C3F5285C8FC2E93F01E90300000CEB6B02B17C4E40A0843EFD0C3D40C01F85EB51B89E134001E9030000389AC8714C7C4E4099620E828E3D40C08FC2F5285C8FE63F01E90300006F0091D9B47D4E40FD0C141D6E3D40C05C8FC2F5285CE3BF01E9030000B0B14B546F7D4E40E458CD29B73D40C0F6285C8FC2F5EC3F01E9030000395C5084797C4E408DE7D8E66B3D40C048E17A14AE47FB3F01E9030000B7584057117C4E4060F93DFBDB3D40C07B14AE47E19A334001E90300009FA40A90B27C4E40CBEC985F723E40C0E17A14AE4701364001E9030000C0A61831FD7B4E4085025BCA9E3E40C0AE47E17A14CE304001E90300006E44413A867C4E40085163E7123F40C048E17A14AE47F33F01E9030000CD2E7315E67E4E405C5C3EED4B4040C0EC51B81E85EBD9BF01E9030000B36213F4727F4E402F9C10DF2E4040C0D7A3703D0AD70C4001E90300008669296A7A7D4E4091DC50E7403F40C05C8FC2F5285CE33F01E9030000C19D55B0D77E4E40ED783826304040C0F6285C8FC2F5EC3F01E90300006032FA76B77E4E40CB4DD4D2DC3D40C0AE47E17A144E314001E9030000E882FA96397E4E401CA90881A63D40C0B81E85EB5138194001E9030000B14D74A7857F4E40E57E87A2403B40C0713D0AD7A300474001E90300001383C0CAA17D4E400EB67D34413B40C0E17A14AE4701354001E903000020178B95877E4E40B10E362B913E40C0E17A14AE4701384001E9030000E626C522E17E4E40E5B27680CC4240C0EC51B81E85EBD93F01E90300004CD0155BF77E4E40CCA843B8134440C0B81E85EB5138194001E9030000F64883DBDA7C4E40B2E20ADE464240C0C3F5285C8FC2E9BF01E9030000B05DDA70587F4E4083932353F44340C01F85EB51B89E114001E9030000676E7484C27C4E406D61CC4C1F3E40C048E17A14AE67324001E90300003EE2A139FC7C4E4078FB62A5384440C048E17A14AE67334001E903000047776BF4C57B4E4019C0A5AD5B4240C048E17A14AE47F33F01E903000007A458C9227E4E409B7FAAC07F4240C0E17A14AE47E1BABF01E903000022BB1CC06C7E4E4055BFD2F9F03C40C0E17A14AE4701394001E90300001CBA34D99A7D4E402C55585F133E40C0C3F5285C8F02284001E9030000B3593F47F57C4E40A9AA2BFA9E3E40C08FC2F5285CCF2E4001E90300008F064BD0BA7E4E407C55C26E333E40C048E17A14AE67324001E9030000CC327A24147E4E40658A39083A3E40C07B14AE47E19A304001E903000005ED1F555E7E4E40888F3E9C763B40C0713D0AD7A380414001E90300000327367A907C4E4095C3DD0F2E4340C0EC51B81E856B1E4001E9030000862EF2FCB37D4E405DCBBFF1104440C05C8FC2F5289C284001E90300006919F3846F7C4E40ADD62E127F4340C0F6285C8FC235274001E9030000E126A3CA307C4E40105839B4C84340C048E17A14AE67314001E90300005EDE7767487F4E409234C9D9853A40C0713D0AD7A380414001E9030000EC5E3821BE7B4E40A4213E0B9D3E40C0EC51B81E856B1A40";
	//		auto wkb = LatLonUtility::HexStringToWKB(hex);
	//		frPick->m_geometry->ImportFromWkb(wkb, hex.length() / 2);
	//		frPick->m_geometry->CreateD2Geometry(theApp.gisLib->D2.pD2Factory);

	//		theApp.gisLib->S101RebuildPortrayal();

	//		delete[] wkb;
	//		MapRefresh();
	//	}
	//}

	Invalidate(FALSE);
}

void COpenS100View::ESC()
{
	frPick = nullptr;

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
		CopyLayer();
		break;
	case VK_F5:
		TestGISLibrary::TestCreateNewCode();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenS100View::PointFeatureList()
{
	if (theApp.gisLib)
	{
		auto fc = theApp.gisLib->GetFC();
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
		auto fc = theApp.gisLib->GetFC();
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
		auto fc = theApp.gisLib->GetFC();
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
	auto layer1 = (S101Layer*)theApp.gisLib->GetLayer(0);
	auto layer2 = (S101Layer*)theApp.gisLib->GetLayer(1);

	auto enc1 = (S101Cell*)layer1->GetSpatialObject();
	auto enc2 = (S101Cell*)layer2->GetSpatialObject();

	S101Creator creator(layer1->GetFeatureCatalog(), enc1);

	auto enc2Features = enc2->GetVecFeature();
	for (auto i = enc2Features.begin(); i != enc2Features.end(); i++)
	{
		auto feature = *i;
		auto newFeature = creator.AddFeature(std::wstring(enc2->m_dsgir.GetFeatureCode(feature->GetNumericCode())));
		if (feature->m_geometry)
		{
			unsigned char* wkb = nullptr;
			int sizeWKB = 0;
			if (feature->m_geometry->ExportToWkb(&wkb, &sizeWKB))
			{
				if (feature->m_geometry->type == 1)
				{
					creator.SetPointGeometry(newFeature, wkb, sizeWKB);
				}
			}
		}
	}

	enc1->Save(L"..\\TEMP\\edit.000");
}