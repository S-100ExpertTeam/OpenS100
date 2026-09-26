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
	DDX_Control(pDX, IDC_MFCPROPERTYGRID_PC_SETTING, portrayalCataloguePropertyGrid);
}


BEGIN_MESSAGE_MAP(CDlg_Portrayal, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_Portrayal::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlg_Portrayal::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlg_Portrayal 메시지 처리기

void CDlg_Portrayal::OnBnClickedOk()
{
}

void CDlg_Portrayal::OnBnClickedCancel()
{
}

void CDlg_Portrayal::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CDlg_Portrayal::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	const int bottomMargin = 7;
	const int bottom = rectClient.bottom - bottomMargin;

	if (listPC.GetSafeHwnd())
	{
		CRect rectList;
		listPC.GetWindowRect(rectList);
		ScreenToClient(rectList);
		rectList.bottom = bottom;
		listPC.MoveWindow(rectList);
	}

	if (portrayalCataloguePropertyGrid.GetSafeHwnd())
	{
		CRect rectPropertyGrid;
		portrayalCataloguePropertyGrid.GetWindowRect(rectPropertyGrid);
		ScreenToClient(rectPropertyGrid);
		rectPropertyGrid.bottom = bottom;
		portrayalCataloguePropertyGrid.MoveWindow(rectPropertyGrid);
	}
}

void CDlg_Portrayal::InitPortrayalCataloguePropertyGrid()
{
	portrayalCataloguePropertyGrid.EnableHeaderCtrl(FALSE);
	portrayalCataloguePropertyGrid.EnableDescriptionArea();
	portrayalCataloguePropertyGrid.SetVSDotNetLook();
	portrayalCataloguePropertyGrid.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Portrayal Catalogue Settings"));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Product ID"), (_variant_t)_T("-"), L"Selected portrayal catalogue product identifier"));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Version"), (_variant_t)_T("-"), L"Selected portrayal catalogue version"));
	portrayalCataloguePropertyGrid.AddProperty(pGroup);
}

BOOL CDlg_Portrayal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	listPC.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InitPortrayalCataloguePropertyGrid();

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

	AdjustLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
