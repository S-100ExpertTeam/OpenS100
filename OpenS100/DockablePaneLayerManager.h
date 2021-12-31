#pragma once

class COpenS100View;
class CDialogDockLayerManager;

// CDockablePaneLayerManager

class CDockablePaneLayerManager : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneLayerManager)

public:
	CDockablePaneLayerManager();
	virtual ~CDockablePaneLayerManager();

public:
	COpenS100View			*pView;
	CDialogDockLayerManager *pDlg;

public:
	void RemoveAllPropList();
	BOOL CanBeClosed() const;
	BOOL CanBeResized() const;
	BOOL CanFloat() const;
	BOOL CanAutoHide() const;
	void UpdateList();
	void DeleteLayer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	virtual void OnFinalRelease();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEdit32913();
};