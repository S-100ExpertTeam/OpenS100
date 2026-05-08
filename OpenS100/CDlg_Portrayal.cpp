// CDlg_Portrayal.cpp: 구현 파일
//

#include "pch.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "CDlg_Portrayal.h"

#include "..\\GISLibrary\\GISLibrary.h"

// CDlg_Portrayal 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Portrayal, CDialogEx)

CDlg_Portrayal::CDlg_Portrayal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlg_Portrayal, pParent)
{

}

CDlg_Portrayal::~CDlg_Portrayal()
{
}

void CDlg_Portrayal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PC, listPC);
}


BEGIN_MESSAGE_MAP(CDlg_Portrayal, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Portrayal::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlg_Portrayal::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlg_Portrayal 메시지 처리기

void CDlg_Portrayal::OnBnClickedOk()
{
}

void CDlg_Portrayal::OnBnClickedCancel()
{
}

BOOL CDlg_Portrayal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	listPC.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	listPC.InsertColumn(0, _T("Product ID"), LVCFMT_LEFT, 100);
	listPC.InsertColumn(1, _T("Version"), LVCFMT_LEFT, 100);

	auto cm = theApp.gisLib->GetCatalogManager();
	if (cm) {
		auto list = cm->getPcProductList();
		int i = 0;
		for (auto pcID : list) {
			CString strID;
			CString strVersion;

			auto pc = cm->getPC(pcID);
			if (pc) {
				strID = pc->GetProductW().c_str();
				strVersion = pc->GetVersionW().c_str();
				listPC.InsertItem(i, strID);
				listPC.SetItemText(i, 1, strVersion);
				i++;
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
