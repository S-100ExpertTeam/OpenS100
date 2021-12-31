#include "pch.h"
#include "DialogDockLogger.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogDockLogger, CDialog)
CComboBox CDialogDockLogger::m_ComboLogType;
CListCtrl CDialogDockLogger::m_ListLogger;

CDialogDockLogger::CDialogDockLogger(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDockLogger::IDD, pParent)
{
	nSelectedItem = -1;
}

CDialogDockLogger::~CDialogDockLogger()
{

}

void CDialogDockLogger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGGER, m_ListLogger);
	DDX_Control(pDX, IDC_COMBO_LOG_TYPE, m_ComboLogType);
	DDX_Control(pDX, IDC_BUTTON_LISTCLEAR, b_ListClearButton);
}

BEGIN_MESSAGE_MAP(CDialogDockLogger, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

// CDialogDockLogger message handlers
int CDialogDockLogger::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	CRect rectDummy;
	rectDummy.SetRectEmpty();
	return 0;
}

void CDialogDockLogger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CDialogDockLogger::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
}