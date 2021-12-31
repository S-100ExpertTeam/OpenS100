#pragma once
#define MYCOMBO1_ID 1000
class COpenS100View;
class CDialogDockLogger;

class CDockablePaneLogger : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneLogger)
	 
public:
	COpenS100View			*pView;
	CDialogDockLogger		*pDlg;

public:
	BOOL CanBeClosed() const;
	BOOL CanBeResized() const;
	BOOL CanFloat() const;
	BOOL CanAutoHide() const;
	CDockablePaneLogger();
	virtual ~CDockablePaneLogger();
	CListCtrl* GetLogListView();

protected:
	DECLARE_MESSAGE_MAP()
    void adjustLayout();
	// Since it wasn't made by the resource editor,
	// It is necessary to adjust the layout according to the initial value and the size of the pane.

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	virtual void OnFinalRelease();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};