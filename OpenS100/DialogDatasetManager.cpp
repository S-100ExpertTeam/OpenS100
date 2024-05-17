
#include "pch.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "DialogDatasetManager.h"



IMPLEMENT_DYNAMIC(DialogDatasetManager, CDialogEx)

DialogDatasetManager::DialogDatasetManager(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DATASET_MANAGER, pParent)
{
	
}

DialogDatasetManager::~DialogDatasetManager()
{
}

void DialogDatasetManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIG_TAB, m_tab);
}


BEGIN_MESSAGE_MAP(DialogDatasetManager, CDialogEx)
	ON_BN_CLICKED(IDOK, &DialogDatasetManager::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONFIG_TAB, &DialogDatasetManager::OnTcnSelchangeConfigTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CONFIG_TAB, &DialogDatasetManager::OnTcnSelchangingConfigTab)
END_MESSAGE_MAP()




BOOL DialogDatasetManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString    strOne = _T("FC / PC");
	CString    strS101 = _T("Datasets / SupportFiles");

	m_tab.InsertItem(1, strOne);
	m_tab.InsertItem(2, strS101);

	CRect r;
	m_tab.GetWindowRect(&r);
	
	// Main
	mainTab.Create(IDD_DIALOG_FC_PC, &m_tab);
	mainTab.ct = ct;
	mainTab.ShowWindow(SW_SHOW);
	mainTab.GetWindowRect(&r);
	mainTab.MoveWindow(5, 25, r.Width(), r.Height());
	//mainTab.ta

	//// S-101
	DSTab.Create(IDD_DIALOG_DATASET_SUPPORT, &m_tab);
	DSTab.ShowWindow(SW_HIDE);
	mainTab.GetWindowRect(&r);
	DSTab.MoveWindow(5, 25, r.Width(), r.Height());

	//s101Tab.m_pParent = this;

	m_tab.SetCurSel(0);
	//mainTab.m_pParent = this;

	return true;
}


void DialogDatasetManager::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void DialogDatasetManager::OnTcnSelchangeConfigTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_tab.GetCurSel(); // After the change, the selected tap number

	switch (nTab)
	{
	case 0:
		mainTab.ShowWindow(SW_SHOW);
		mainTab.SetRefreshItem();
		break;
	case 1:
		DSTab.ShowWindow(SW_SHOW);
		DSTab.SetRefreshItem();
		break;

	}

	Invalidate(true);
	*pResult = 0;
}


void DialogDatasetManager::OnTcnSelchangingConfigTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_tab.GetCurSel(); // Tab number selected before it changes

	switch (nTab)
	{
	case 0:
		mainTab.ShowWindow(SW_HIDE);
		break;
	case 1:
		DSTab.ShowWindow(SW_HIDE);
		break;

	}
	*pResult = 0;
}
