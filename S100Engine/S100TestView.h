#pragma once

#include "S100DrawingUnit.h"
#include "SVGReader.h"
#include "AreaFillInfo.h"

class CS100TestView : public CView
{
protected: // create from serialization only
	CS100TestView();
	DECLARE_DYNCREATE(CS100TestView)

// Attributes
public:
	CS100TestDoc* GetDocument() const;

	ID2D1Factory1* m_pDirect2dFactory;

	ID2D1StrokeStyle1* m_pStrokeStyleS101Solid;
	ID2D1StrokeStyle1* m_pStrokeStyleS101Dash;
	ID2D1StrokeStyle1* m_pStrokeStyleS101Ray;

	IDWriteFactory* _pDWriteFactory = nullptr;
	IDWriteTextFormat* _pTextFormat = nullptr;

	ID2D1DCRenderTarget* m_pRenderTarget;

	ID2D1SolidColorBrush* m_pBrush;

	IWICImagingFactory* m_pImagingFactory;
	
	int dpiX;
	int dpiY;

	// Test - Line Style
	void VarInitTestLineStyleResource();
	void DiscardTestLineStyleResource();

	ID2D1PathGeometry *m_pTestPolygonGeometry;
	IWICBitmap *m_pTestWICBitmap;

	

// Operations
public:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(CDC* pDC, CRect& rect);
	HRESULT CreateDeviceIndependentTextResource();
	void DiscardDeviceIndependentResource();
	void DiscardDeviceResources();
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();
	virtual ~CS100TestView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in S100TestView.cpp
inline CS100TestDoc* CS100TestView::GetDocument() const
   { return reinterpret_cast<CS100TestDoc*>(m_pDocument); }
#endif