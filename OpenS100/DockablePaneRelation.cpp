// DockablePaneRelation.cpp : implementation file
//
#include "pch.h"
#include "DockablePaneRelation.h"
#include "DialogDockRelation.h"

IMPLEMENT_DYNAMIC(CDockablePaneRelation, CDockablePane)

CDockablePaneRelation::CDockablePaneRelation()
{
	EnableAutomation();
	pDlg = nullptr;
}

CDockablePaneRelation::~CDockablePaneRelation()
{
	delete pDlg;
}

BEGIN_MESSAGE_MAP(CDockablePaneRelation, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CDockablePaneRelation::OnPaint()
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

int CDockablePaneRelation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rect;
	this->GetWindowRect(&rect);

	pDlg = new CDialogDockRelation();
	pDlg->Create(IDD_DIALOG_DOCK_RELATION, this);
	pDlg->InvalidateRect(NULL, TRUE);
	pDlg->ShowWindow(SW_SHOW);

	return 0;
}

void CDockablePaneRelation::OnSize(UINT nType, int cx, int cy)
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

void CDockablePaneRelation::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here

	/*
	** Dialog
	*/
	pDlg->SetFocus();
}

void CDockablePaneRelation::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CDockablePaneRelation::CanBeClosed() const
{
	return TRUE;
}

BOOL CDockablePaneRelation::CanBeResized() const
{
	return TRUE;
}

BOOL CDockablePaneRelation::CanFloat() const
{
	return TRUE;
}

BOOL CDockablePaneRelation::CanAutoHide() const
{
	return TRUE;
}

void CDockablePaneRelation::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class

	CDockablePane::OnFinalRelease();
}

BOOL CDockablePaneRelation::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDockablePane::PreCreateWindow(cs);
}

void CDockablePaneRelation::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	return;
}

void CDockablePaneRelation::UpdateList()
{
	pDlg->UpdateList();
}