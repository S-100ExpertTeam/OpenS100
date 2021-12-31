// DockablePaneLogger.cpp : implementation file
//
#include "pch.h"
#include "DockablePaneLogger.h"
#include "DialogDockLogger.h"

IMPLEMENT_DYNAMIC(CDockablePaneLogger, CDockablePane)

CDockablePaneLogger::CDockablePaneLogger()
{
	EnableAutomation();
	pDlg = nullptr;
}


CDockablePaneLogger::~CDockablePaneLogger()
{
	if (pDlg != nullptr)
		delete pDlg;
}

BEGIN_MESSAGE_MAP(CDockablePaneLogger, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CDockablePaneLogger::OnPaint()
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



int CDockablePaneLogger::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

/*
** Attach Dialog to the dockable pane.
*/
	CRect rect;
	this->GetWindowRect(&rect);
	//===========================================

	pDlg = new CDialogDockLogger();
	pDlg->Create(IDD_DIALOG_DOCK_LOGGER, this);
	pDlg->InvalidateRect(NULL, TRUE);
	pDlg->ShowWindow(SW_SHOW);

	return 0;
}

void CDockablePaneLogger::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	/*
	** Dialog
	*/
	CRect rect;
	this->GetWindowRect(rect);
	pDlg->MoveWindow(0, 0, cx, cy);
	adjustLayout();
}


void CDockablePaneLogger::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here

	/*
	** Dialog
	*/
	pDlg->SetFocus();
}

void CDockablePaneLogger::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CDockablePaneLogger::CanBeClosed() const
{
	return TRUE;
}


BOOL CDockablePaneLogger::CanBeResized() const
{
	return TRUE;
}


BOOL CDockablePaneLogger::CanFloat() const
{
	return TRUE;
}


BOOL CDockablePaneLogger::CanAutoHide() const
{
	return TRUE;
}

void CDockablePaneLogger::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class

	CDockablePane::OnFinalRelease();
}


BOOL CDockablePaneLogger::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDockablePane::PreCreateWindow(cs);
}


void CDockablePaneLogger::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	return;
}


CListCtrl* CDockablePaneLogger::GetLogListView()
{
	return &pDlg->m_ListLogger;
}

void CDockablePaneLogger::adjustLayout()
{
	if (GetSafeHwnd() == NULL) return;
}