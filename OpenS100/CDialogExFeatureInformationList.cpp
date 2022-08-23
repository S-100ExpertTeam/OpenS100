// CDialogExFeatureInformationList.cpp: 구현 파일
//

#include "pch.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "CDialogExFeatureInformationList.h"


// CDialogExFeatureInformationList 대화 상자

IMPLEMENT_DYNAMIC(CDialogExFeatureInformationList, CDialogEx)

CDialogExFeatureInformationList::CDialogExFeatureInformationList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDialogExFeatureInformationList, pParent)
{

}

CDialogExFeatureInformationList::~CDialogExFeatureInformationList()
{
}

void CDialogExFeatureInformationList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, comboType);
	DDX_Control(pDX, IDC_COMBO_GEOM, comboGeom);
	DDX_Control(pDX, IDC_LIST, listCtrl);
}


BEGIN_MESSAGE_MAP(CDialogExFeatureInformationList, CDialogEx)
END_MESSAGE_MAP()


// CDialogExFeatureInformationList 메시지 처리기


BOOL CDialogExFeatureInformationList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	comboType.AddString(L"Feature");
	comboType.AddString(L"Information");
	comboType.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
