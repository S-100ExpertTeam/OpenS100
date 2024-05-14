#include "pch.h"
#include "DockablePaneLayerManager.h"
#include "DialogDockLayerManager.h"

// CDockablePaneLayerManager
IMPLEMENT_DYNAMIC(CDockablePaneLayerManager, CDockablePane)

CDockablePaneLayerManager::CDockablePaneLayerManager()
{
	//EnableAutomation();
	pDlg = nullptr;
}

CDockablePaneLayerManager::~CDockablePaneLayerManager()
{
	delete pDlg; 
}

BEGIN_MESSAGE_MAP(CDockablePaneLayerManager, CDockablePane)
	ON_WM_CREATE() 
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CDockablePaneLayerManager message handlers
int CDockablePaneLayerManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  Add your specialized creation code here

	CRect rect;
	this->GetWindowRect(&rect);
	
	pDlg = new CDialogDockLayerManager();
	pDlg->Create(IDD_DIALOG_DOCK_LAYERMANEGER, this);
	pDlg->InvalidateRect(NULL, TRUE);
	pDlg->ShowWindow(SW_SHOW);
	
	return 0;
}


void CDockablePaneLayerManager::OnPaint()
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


void CDockablePaneLayerManager::OnSize(UINT nType, int cx, int cy)
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


void CDockablePaneLayerManager::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here

	/*
	** Dialog
	*/
	pDlg->SetFocus();
}

void CDockablePaneLayerManager::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CDockablePaneLayerManager::CanBeClosed() const
{
	return TRUE;
}


BOOL CDockablePaneLayerManager::CanBeResized() const
{
	return true;
}

void CDockablePaneLayerManager::UpdateList()
{
	pDlg->UpdateList();
}

BOOL CDockablePaneLayerManager::CanFloat() const
{
	return true;
}


BOOL CDockablePaneLayerManager::CanAutoHide() const
{
	return true;
}

void CDockablePaneLayerManager::OnFinalRelease()
{
	// TODO: Add your specialized code here and/or call the base class

	CDockablePane::OnFinalRelease();
}


BOOL CDockablePaneLayerManager::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDockablePane::PreCreateWindow(cs);
}


void CDockablePaneLayerManager::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	return;
}



void CDockablePaneLayerManager::RemoveAllPropList()
{
	pDlg->RemoveAllPropList();
}


void CDockablePaneLayerManager::DeleteLayer() 
{
	pDlg->DeleteLayer();
}

void CDockablePaneLayerManager::OnEdit32913()
{

}
