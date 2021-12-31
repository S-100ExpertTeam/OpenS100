// DockablePaneRelation.cpp : implementation file
//
#include "pch.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "DockablePaneCurrentSelection.h"
#include "DialogDockCurrentSelection.h"

#include <map>

IMPLEMENT_DYNAMIC(CDockablePaneCurrentSelection, CDockablePane)

CDockablePaneCurrentSelection::CDockablePaneCurrentSelection()
{
	EnableAutomation();
	pDlg = nullptr;
}

CDockablePaneCurrentSelection::~CDockablePaneCurrentSelection()
{
	delete pDlg;
}

BEGIN_MESSAGE_MAP(CDockablePaneCurrentSelection, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CDockablePaneCurrentSelection::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDockablePane::OnPaint() for painting messages

	CRect rect;
	this->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.InflateRect(1, 1);
	dc.Draw3dRect(rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
	dc.FillRect(rect, &CBrush(RGB(230, 230, 230))); // make the background gray.
}


int CDockablePaneCurrentSelection::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	pDlg = new CDialogDockCurrentSelection();
	pDlg->Create(IDD_DIALOG_DOCK_CURRENT_SELECTION, this);
	pDlg->InvalidateRect(NULL, TRUE);
	pDlg->ShowWindow(SW_SHOW);

	return 0;
}



void CDockablePaneCurrentSelection::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	/*
	** Dialog
	*/
	CRect rect;
	this->GetWindowRect(rect);
	pDlg->MoveWindow(0, 0, cx, cy);
}

void CDockablePaneCurrentSelection::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CDockablePaneCurrentSelection::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CDockablePaneCurrentSelection::CanBeClosed() const
{
	return TRUE;
}


BOOL CDockablePaneCurrentSelection::CanBeResized() const
{
	return TRUE;
}


BOOL CDockablePaneCurrentSelection::CanFloat() const
{
	return TRUE;
}


BOOL CDockablePaneCurrentSelection::CanAutoHide() const
{
	return TRUE;
}

void CDockablePaneCurrentSelection::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class

	CDockablePane::OnFinalRelease();
}


BOOL CDockablePaneCurrentSelection::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDockablePane::PreCreateWindow(cs);
}


void CDockablePaneCurrentSelection::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	return;
}


void CDockablePaneCurrentSelection::UpdateList()
{
	pDlg->UpdateList();
}

void CDockablePaneCurrentSelection::RemoveAll()
{
	pDlg->OnMenuRemoveAll();
}

void CDockablePaneCurrentSelection::DeleteItem(CString id) 
{
	pDlg->DeleteItem(id);
}

void CDockablePaneCurrentSelection::UpdateListTest(CStringArray *csa, S101Cell *cell, CString isCtrlClicked)
{
	pDlg->UpdateListTest(csa, cell, isCtrlClicked);
}