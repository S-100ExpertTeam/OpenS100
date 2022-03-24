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

#include "..\\GISLibrary\\GISLibrary.h"
#include "..\\GISLibrary\\Layer.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\GISLibrary\\CodeWithNumericCode.h"

#include "..\\GeoMetryLibrary\\GeometricFuc.h"
#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"
#include "..\\GeoMetryLibrary\\Scaler.h"
#include "..\\GeoMetryLibrary\\GeoPolyline.h"


#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SMultiPoint.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SGeometricFuc.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

#include <locale>        
#include <codecvt>       
#include <d2d1_1.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <crtdbg.h>
#include <iostream>


#include "..\\LatLonUtility\\LatLonUtility.h"
#pragma comment(lib, "d2d1.lib")

using namespace LatLonUtility;

class CMainFrame;
// COpenS100View

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
//	ON_WM_MOUSEHOVER()
ON_WM_NCMOUSEHOVER()
ON_WM_MOUSELEAVE()
//ON_WM_SYSKEYDOWN()
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

	delete gisLib;
	gisLib = nullptr;

	CoUninitialize();
}

void COpenS100View::SaveLastPosScale()
{
	double scale = gisLib->GetCurrentScale();
	int sox = gisLib->GetCenterXScreen();
	int soy = gisLib->GetCenterYScreen();

	MBR mbr;
	gisLib->GetMap(&mbr);

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
	if (file.Open(_T("..\\ProgramData\\data\\init.txt"), CFile::modeWrite | CFile::modeCreate))
	{
		file.WriteString(strScale);
		file.WriteString(strSox);
		file.WriteString(strSoy);
	}
	file.Close();
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
	gisLib->SetViewMBR(rect);

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

			m_strFormatedScale = gisLib->GetScaler()->GetFormatedScale();
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

	gisLib->SetScreen(viewRect);
	gisLib->ZoomOut(0, viewRect.Width() / 2, viewRect.Height() / 2);
	gisLib->UpdateScale();

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
	CString strFileList;

	bool addlayer = false;

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();

		RemoveLoadFile(); //Delete the existing history.
		addlayer = gisLib->AddLayer(filePath); //Add a layer.
		theApp.m_pDockablePaneLayerManager.UpdateList();
		MapRefresh();
	}
}

void COpenS100View::RemoveLoadFile()
{
	theApp.m_pDockablePaneLayerManager.DeleteLayer();
}

void COpenS100View::MapPlus()
{
	CRect rect;
	GetClientRect(rect);
	gisLib->ZoomIn(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	MapRefresh();
}

void COpenS100View::MapMinus()
{
	CRect rect;
	GetClientRect(rect);
	gisLib->ZoomOut(ZOOM_FACTOR, rect.Width() / 2, rect.Height() / 2);
	MapRefresh();
}

void COpenS100View::MapDragSize()
{
	(m_Icon == ZOOM_AREA) ? m_Icon = MOVE : m_Icon = ZOOM_AREA;
}

void COpenS100View::MapFill()
{
	Layer *layer = gisLib->GetLayer();
	if (nullptr == layer)
	{
		return;
	}

	auto layerMBR = layer->GetMBR();

	gisLib->GetLayerManager()->GetScaler()->SetMap(layerMBR);
	gisLib->AdjustScreenMap();
	theApp.MapRefresh();
}

void COpenS100View::NoGeometry()
{
	auto layer = GetCurrentLayer();
	if (layer == nullptr)
	{
		//if layer than nullptr.
		AfxMessageBox(L"Please Open a layer");
		return;
	}
	S101Cell* cell = (S101Cell*)layer->m_spatialObject;

	CDialogViewNoGeometry*  dlg = new CDialogViewNoGeometry(this);
	dlg->SetNoGeometryFeatureList(cell);
	dlg->Create(IDD_DIALOG_NOGEOMETRY);
	dlg->ShowWindow(SW_SHOW);
}

void COpenS100View::NoGeometryInfo()
{

	auto layer = GetCurrentLayer();
	if (layer == nullptr)
	{
		//if layer than nullptr
		AfxMessageBox(L"Please Open a layer");
		return;
	}
	S101Cell* cell = (S101Cell*)layer->m_spatialObject;
	CDialogViewInformationType*  dlg = new CDialogViewInformationType(this);
	dlg->SetInformationFeatureList(cell);
	dlg->Create(IDD_DIALOG_INFORMATIONTYPE);
	dlg->ShowWindow(SW_SHOW);
}

void COpenS100View::Setting()
{
	CConfigrationDlg dlg(this);

	auto fc = gisLib->GetFC();
	if (fc)
	{
		dlg.InitS101FeatureTypes(fc);
	}

	if (m_systemFontList.size() == 0)
	{
		// <FONT LIST>
		HRESULT hr;
		IDWriteFactory* pDWriteFactory = gisLib->D2.pDWriteFactory;
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
	gisLib->AddBackgroundLayer(_T("..\\ProgramData\\World\\World.shp"));

	MapRefresh();
}

int COpenS100View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	gisLib->InitLibrary();

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
	gisLib->DeviceToWorld(cr.Width() / 2, cr.Height() / 2, &moveMX, &moveMY);

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

	if (gisLib->GetScaler()->GetRotationDegree())
	{
		double radian = (180 + gisLib->GetScaler()->GetRotationDegree()) * DEG2RAD;

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
		gisLib->MoveMap(cr.Width() / 2 + dx, cr.Height() / 2 + dy, moveMX, moveMY);
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

				gisLib->DeviceToWorld(
					m_ptStartZoomArea.x,
					m_ptStartZoomArea.y,
					&startX,
					&startY);

				gisLib->DeviceToWorld(
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

				gisLib->SetMap(xmin, ymin, xmax, ymax);

				CRect viewRect;
				GetClientRect(viewRect);

				gisLib->SetScreen(viewRect);
				gisLib->ZoomOut(0, viewRect.Width() / 2, viewRect.Height() / 2);
				gisLib->UpdateScale();

				gisLib->AdjustScreenMap();

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
	gisLib->DeviceToWorld(point.x, point.y, &curLong, &curLat);
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

	gisLib->Draw(hdc);

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
	return gisLib->GetLayer();
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
		gisLib->ZoomIn(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
	}
	// When you lower the mouse wheel => Zoom out.
	else
	{
		gisLib->ZoomOut(ZOOM_FACTOR, m_ptCurrent.x, m_ptCurrent.y);
	}

	MapRefresh();


	m_strFormatedScale = gisLib->GetScaler()->GetFormatedScale();
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

		gisLib->WorldToDevice(((SPoint*)(frPick->m_geometry))->m_point.x, ((SPoint*)(frPick->m_geometry))->m_point.y, &x, &y);
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

			gisLib->WorldToDevice(multiPoint->GetX(i), multiPoint->GetY(i), &x, &y);
			x += offsetX;
			y += offsetY;

			g.DrawEllipse(&Pen(Color(255, 0, 0), 4), x - 20, y - 20, 40, 40);
		}
	}
	// Line
	else if (frPick->m_geometry->IsCurve())
	{
		SolidBrush brush(Color(255, 0, 0));

		SCompositeCurve* cc = (SCompositeCurve*)(frPick->m_geometry);

		for (auto it = cc->m_listCurveLink.begin(); it != cc->m_listCurveLink.end(); it++)
		{
			SCurve* c = (*it).GetCurve();
			Gdiplus::Point* pickPoints = new Gdiplus::Point[c->m_numPoints];

			int pickNumPoints = 0;

			pickNumPoints = c->GetNumPoints();

			if ((*it).GetParentOrient()) {
				for (auto i = 0; i < pickNumPoints; i++)
				{
					pickPoints[i].X = (INT)c->m_pPoints[i].x;
					pickPoints[i].Y = (INT)c->m_pPoints[i].y;
					gisLib->WorldToDevice(c->m_pPoints[i].x, c->m_pPoints[i].y,
						(long*)(&pickPoints[i].X), (long*)(&pickPoints[i].Y));

					pickPoints[i].X += offsetX;
					pickPoints[i].Y += offsetY;
				}
			}
			else
			{
				for (auto i = 0; i < pickNumPoints; i++)
				{
					pickPoints[i].X = (INT)c->m_pPoints[pickNumPoints - 1 - i].x;
					pickPoints[i].Y = (INT)c->m_pPoints[pickNumPoints - 1 - i].y;
					gisLib->WorldToDevice(c->m_pPoints[i].x, c->m_pPoints[i].y,
						(long*)(&pickPoints[i].X), (long*)(&pickPoints[i].Y));

					pickPoints[i].X += offsetX;
					pickPoints[i].Y += offsetY;
				}
			}
			g.DrawLines(&Pen(&brush, 4), pickPoints, pickNumPoints);

			delete[] pickPoints;
		}
	}
	// Area
	else if (frPick->m_geometry->IsSurface())
	{
		auto surface = (SSurface*)frPick->m_geometry;
		auto geometry = surface->GetNewD2Geometry(gisLib->D2.pD2Factory, gisLib->GetScaler());

		D2D1_COLOR_F color{};
		color.r = 255 / float(255.0);
		color.g = 50 / float(255.0);
		color.b = 50 / float(255.0);
		color.a = float(0.7);

		HDC hdc = g.GetHDC();
		gisLib->D2.Begin(hdc);
		if (geometry)
		{
			gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(float(offsetX), float(offsetY)));
			gisLib->D2.pBrush->SetColor(color);
			gisLib->D2.pBrush->SetOpacity(float(0.7));
			gisLib->D2.pRT->FillGeometry(geometry, gisLib->D2.pBrush);
			SafeRelease(&geometry);
		}
		gisLib->D2.End();
	}
}

void COpenS100View::ClearPickReport()
{
	frPick = nullptr;
}

void COpenS100View::PickReport(CPoint _point)
{
	auto layer = gisLib->GetLayerManager()->GetLayer();
	if (nullptr == layer)
	{
		return;
	}
	
	auto cell = (S101Cell*)layer->GetSpatialObject();
	if (nullptr == cell)
	{
		return;
	}

	gisLib->DeviceToWorld(_point.x, _point.y, &ptPickX, &ptPickY);
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
		gisLib->DeviceToWorld(_point.x - 5, _point.y + 5, &xmin, &ymin);  // start point
		gisLib->DeviceToWorld(_point.x + 5, _point.y - 5, &xmax, &ymax);  // end point
	}
	// square
	else
	{
		if (spt_x < ept_x && spt_y > ept_y)
		{
			gisLib->DeviceToWorld(spt_x, ept_y, &xmin, &ymax);
			gisLib->DeviceToWorld(ept_x, spt_y, &xmax, &ymin);
		}
		else if (spt_x < ept_x && spt_y < ept_y)
		{
			gisLib->DeviceToWorld(spt_x, spt_y, &xmin, &ymax);
			gisLib->DeviceToWorld(ept_x, ept_y, &xmax, &ymin);
		}

		else if (spt_x > ept_x && spt_y > ept_y)
		{
			gisLib->DeviceToWorld(ept_x, ept_y, &xmin, &ymax);
			gisLib->DeviceToWorld(spt_x, spt_y, &xmax, &ymin);
		}

		else if (spt_x > ept_x && spt_y < ept_y)
		{
			gisLib->DeviceToWorld(ept_x, spt_y, &xmin, &ymax);
			gisLib->DeviceToWorld(spt_x, ept_y, &xmax, &ymin);
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

		SSurface *surface = (SSurface *)fr->m_geometry;

		if (MBR::CheckOverlap(pickMBR, fr->m_geometry->m_mbr))
		{
			int code = fr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);

			double centerX = 0;
			double centerY = 0;

			gisLib->DeviceToWorld(_point.x, _point.y, &centerX, &centerY);

			if (SGeometricFuc::inside(centerX, centerY, (SSurface*)surface) == 1)
			{
				CString csFrid;
				CString csFoid;
				CString csLat;
				CString csLon;
				CString csType;
				CString csName;
				CString csAssoCnt;

				double lat = surface->m_pPoints[0].x;
				double lon = surface->m_pPoints[0].y;

				inverseProjection(lat, lon);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->m_fasc.size() + fr->m_inas.size();

				csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
				csFoid.Format(_T("%d"), fr->m_foid.m_objName.m_fidn);
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

	pos = cell->GetFeatureStartPosition();
	while (pos != NULL)
	{
		cell->GetNextAssoc(pos, key, fr);
		if (fr->m_geometry == nullptr || fr->m_geometry->type != 2)
		{
			continue;
		}

		SCompositeCurve *compositeCurve = (SCompositeCurve *)fr->m_geometry;
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
					lat = ((*compositeCurve->m_listCurveLink.begin()).GetCurve())->m_pPoints[0].x;
					lon = ((*compositeCurve->m_listCurveLink.begin()).GetCurve())->m_pPoints[0].y;
				}

				inverseProjection(lat, lon);

				std::vector<int>::size_type assoCnt;
				assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

				csFoid.Format(_T("%d"), fr->m_foid.m_objName.m_fidn);
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

		SGeometry *sgeo = (SGeometry *)fr->m_geometry;
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

						csFoid.Format(_T("%d"), fr->m_foid.m_objName.m_fidn);
						csFrid.Format(_T("%d"), fr->m_frid.m_name.RCID);
						csLat.Format(_T("%f"), geoX);
						csLon.Format(_T("%f"), geoY);
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
				double geoX = ((SPoint*)fr->m_geometry)->m_point.x;
				double geoY = ((SPoint*)fr->m_geometry)->m_point.y;

				if (((pickMBR.xmin <= geoX) && (geoX <= pickMBR.xmax))
					&& ((pickMBR.ymin <= geoY) && (geoY <= pickMBR.ymax)))
				{
					CString csFoid, csFrid, csLat, csLon, csType, csName, csAssoCnt;

					SPoint* sr = (SPoint*)fr->m_geometry;
					double lat = sr->m_point.x;
					double lon = sr->m_point.y;

					inverseProjection(lat, lon);

					std::vector<int>::size_type assoCnt;
					assoCnt = fr->m_fasc.size() + fr->m_inas.size();;

					csFoid.Format(_T("%d"), fr->m_foid.m_objName.m_fidn);
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
	// directly send value
	theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, cell, L"0");
	Invalidate(FALSE);
}

void COpenS100View::SetPickReportFeature(R_FeatureRecord* _fr)
{
	frPick = _fr;
	Layer* l = nullptr;

	l = (Layer*)gisLib->GetLayer();

	if (l == NULL)
	{
		CString str;
		str.Format(_T("Layer (%d) could not be retrieved."), 0);
		AfxMessageBox(str);
		return;
	}

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
		gisLib->MoveMap(-10, 0);
		MapRefresh();
		break;
	case VK_RIGHT:
		gisLib->MoveMap(10, 0);
		MapRefresh();
		break;
	case VK_UP:
		gisLib->MoveMap(0, -10);
		MapRefresh();
		break;
	case VK_DOWN:
		gisLib->MoveMap(0, 10);
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
		gisLib->ZoomIn(ZOOM_FACTOR);
		MapRefresh();
		break;
	case VK_NEXT:
		gisLib->ZoomOut(ZOOM_FACTOR);
		MapRefresh();
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
