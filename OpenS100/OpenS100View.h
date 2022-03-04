#pragma once

class COpenS100Doc;
class Layer;
class R_FeatureRecord;
class NewFeatureManager;

class COpenS100View : public CView
{
protected: // It is made only by serialization.
	COpenS100View();
	DECLARE_DYNCREATE(COpenS100View)

public:
	COpenS100Doc* GetDocument() const;

public:
	// If the screen movement has started, the initial value is FALSE according to TRUE or FALSE.
	bool m_bMoveStart = false;
	bool m_bMapRefesh = false;
	
	CDC mem_dc;
	CBitmap memBitmap;
	CDC transDC;
	CBitmap transBitmap;
	CDC map_dc;
	CBitmap mapBitmap;

	volatile int m_Icon = 0;

	// Variables used in the screen movement function.
	// screen movement is calculated based on the center point.
	double moveMX = 0.0;
	double moveMY = 0.0;

	// starting and ending points used for the screen movement function.
	CPoint m_sp;
	CPoint m_ep;

	// If the mouse is clicked, TRUE or FALSE.
	CPoint m_ptCurrent; // OnMouseMove's point Save variable

	CPoint m_ptMDown;   // OnMButtonDown's point Save variables
	CPoint m_ptMUp;     // OnMButtonUp's point Save variables

	// Use it to mark the scale and latitude.
	CString m_strScale;
	CString	m_strLatitude;
	CString	m_strLongitude;
	CString m_strFormatedScale;
	CString m_strFormatedLatitude;
	CString m_strFormatedLongitude;
	

	CPoint ptPick = { -100, -100 };
	bool isMoved = false;

	R_FeatureRecord* frPick = nullptr;  // Feature selected on the screen (S-101)
	double ptPickX = 0.0;
	double ptPickY = 0.0;
	double ptOldPickX = 0.0;
	double ptOldPickY = 0.0;

	// Variables related to mouse change points.
	unsigned int modifyWaypointIndex; // The acupuncture point number that's being fixed with the mouse
	bool beModifyWaypoint = false;    // Pointing to whether you're fixing the turning point with a mouse.

	CPoint m_ptStartZoomArea;
	CPoint m_ptEndZoomArea;
	bool m_bZoomArea = false;

protected:
	std::vector<CString> m_systemFontList;
	// Generated message map functions

public:
	virtual void OnDraw(CDC* pDC);  
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~COpenS100View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void Load100File(); //Add Layer
	afx_msg void RemoveLoadFile(); //Delete Layer
	afx_msg void MapPlus(); //Layer Zoomin
	afx_msg void MapMinus(); //Layer Zoomout
	afx_msg	void MapDragSize(); //
	afx_msg void MapFill();

	afx_msg void NoGeometry();
	afx_msg void NoGeometryInfo();

	afx_msg void Setting();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	void MapRefresh();
	void OpenWorldMap();

	void CreateDCs(CDC* pDC, CRect& rect);
	void DeleteDCs();

	// Zoom Area
	void DrawZoomArea(CDC* pDC);

	void DrawFromMapRefresh(CDC* pDC, CRect& rect);
	void DrawFromInvalidate(CDC* pDC, CRect& rect);

	Layer* GetCurrentLayer();


	void DrawPickReport(HDC& _hdc, int offsetX = 0, int offsetY = 0);
	void DrawS101PickReport(Graphics& g, int offsetX = 0, int offsetY = 0);
	void ClearPickReport();
	void PickReport(CPoint _point);
	void SetPickReportFeature(R_FeatureRecord* _fr);

	// It runs at the end and stores the last location and scale.
	void SaveLastPosScale();
	void ESC();
	
};


#ifndef _DEBUG  // Debug version of OpenS100View.cpp
inline COpenS100Doc* COpenS100View::GetDocument() const
   { return reinterpret_cast<COpenS100Doc*>(m_pDocument); }
#endif

