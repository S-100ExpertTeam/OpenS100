#pragma once
#include <string>
#include <map> 

class COpenS100View;
class CurrentSelection;
class CDialogDockCurrentSelection;
class S101Cell;
class CDockablePaneCurrentSelection : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneCurrentSelection)

public:

	CDockablePaneCurrentSelection();
	virtual ~CDockablePaneCurrentSelection();

public:
	CDialogDockCurrentSelection		*pDlg;

public:
	BOOL CanBeClosed() const;
	BOOL CanBeResized() const;
	BOOL CanFloat() const;
	BOOL CanAutoHide() const;

public:
	void UpdateList();
	void RemoveAll();
	void UpdateListTest(CStringArray *csa, S101Cell *cell, CString isCtrlClicked);
	void DeleteItem(CString id);

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
};