#include "pch.h"
#include "OpenS100.h"
#include "Configuration_4.h"
#include "afxdialogex.h"
#include "ConfigrationDlg.h"
#include "DialogDockLayerManager.h"

#include "../GISLibrary/LayerManager.h"
#include "../GISLibrary/GISLibrary.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

// Configuration_4 dialog box
IMPLEMENT_DYNAMIC(CConfiguration_4, CDialogEx)

CConfiguration_4::CConfiguration_4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CConfiguration_4::IDD, pParent)
{

}

CConfiguration_4::~CConfiguration_4()
{

}

void CConfiguration_4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEWINGGROUP, m_viewingGroupList);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, comboBoxProduct);
}

BEGIN_MESSAGE_MAP(CConfiguration_4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CConfiguration_4::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_UNCHECK, &CConfiguration_4::OnBnClickedButtonUncheck)
END_MESSAGE_MAP()

BOOL CConfiguration_4::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	try
	{
		CRect rect;

		m_viewingGroupList.GetClientRect(rect);
		m_viewingGroupList.InsertColumn(0, _T("On/Off"), LVCFMT_LEFT, 60);
		m_viewingGroupList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100);
		m_viewingGroupList.InsertColumn(2, _T("Code"), LVCFMT_LEFT, 100);
		m_viewingGroupList.InsertColumn(3, _T("Definition"), LVCFMT_LEFT, rect.Width() - 200);

		m_viewingGroupList.SetExtendedStyle(m_viewingGroupList.GetExtendedStyle() | LVS_EX_CHECKBOXES);

		if (theApp.m_pDockablePaneLayerManager.pDlg == nullptr) //If you don't have the opened price,
		{
			return false;
		}

		auto fc = theApp.gisLib->GetFC();
		if (fc == nullptr)
		{
			return false;
		}

		auto vecFeature = fc->GetFeatureTypes().GetVecFeatureType();

		for (int i = 0; i < vecFeature.size(); i++)
		{
			auto feature = vecFeature[i];
			auto name = feature->GetName();
			auto code = feature->GetCodeAsWString();
			auto definition = feature->GetDefinition();

			m_viewingGroupList.InsertItem(i, _T(""));
			m_viewingGroupList.SetItemText(i, 1, name.c_str());
			m_viewingGroupList.SetItemText(i, 2, code.c_str());
			m_viewingGroupList.SetItemText(i, 3, definition.c_str());
		}
		
		//I check all the details.
		int nCount = m_viewingGroupList.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			m_viewingGroupList.SetCheck(i);
		}

		// Combo box (Product)
		comboBoxProduct.AddString(L"S-101");
		comboBoxProduct.SetCurSel(0);

		return true;  // return TRUE unless you set the focus to a control
	}
	catch (int exceptionCode)
	{
		exceptionCode = 0;
		return false;
	}
}

void CConfiguration_4::OnBnClickedButtonCheck()
{
	int cnt = m_viewingGroupList.GetItemCount();
	for (int i = 0; i < cnt; i++)
	{
		m_viewingGroupList.SetCheck(i);
	}
}

void CConfiguration_4::OnBnClickedButtonUncheck()
{
	int cnt = m_viewingGroupList.GetItemCount();
	for (int i = 0; i < cnt; i++)
	{
		m_viewingGroupList.SetCheck(i, FALSE);
	}
}

void CConfiguration_4::OnCancel()
{
	m_pParent->OnBnClickedCancel();
}


void CConfiguration_4::OnOK()
{
	m_pParent->OnBnClickedOk();
}
