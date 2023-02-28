//
#include "pch.h"
#include "afxdialogex.h"
#include "DialogViewInformationType.h"
#include "resource.h"
#include "OpenS100.h"

#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../FeatureCatalog/InformationType.h"

#include "../GISLibrary/S101Cell.h"
#include "../GISLibrary/R_InformationRecord.h"
#include "../GISLibrary/CodeWithNumericCode.h"
#include "../GISLibrary/S100Layer.h"

#include <algorithm>

IMPLEMENT_DYNAMIC(CDialogViewInformationType, CDialogEx)

CDialogViewInformationType::CDialogViewInformationType(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INFORMATIONTYPE, pParent)
{

}

CDialogViewInformationType::~CDialogViewInformationType()
{

}

void CDialogViewInformationType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFORMATIONTYPE, m_ViewListInformationType);
}

BOOL CDialogViewInformationType::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect listRect;
	m_ViewListInformationType.GetWindowRect(listRect);

	m_ViewListInformationType.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ViewListInformationType.InsertColumn(0, _T("ID"), LVCFMT_CENTER, (int)(listRect.Width()*0.20));
	m_ViewListInformationType.InsertColumn(1, _T("Name"), LVCFMT_LEFT, (int)(listRect.Width()*0.30));
	m_ViewListInformationType.InsertColumn(2, _T("Relation Cnt"), LVCFMT_LEFT, (int)(listRect.Width()*0.50));

	return true;
}

void CDialogViewInformationType::SetInformationFeatureList(S100SpatialObject* s100Dataset)
{
	this->s100Dataset = s100Dataset;

	ngflist.clear();
	auto fc = ((S100Layer*)s100Dataset->m_pLayer)->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	int cnt = s100Dataset->GetInformationCount();
	for (int i = 0; i < cnt; i++)
	{
		auto informationType = s100Dataset->GetInformationTypeByIndex(i);
		if (informationType)
		{
			auto code = s100Dataset->GetInformationTypeCodeByID(informationType->GetIDAsInteger());
			auto fcInformationType = fc->GetInformationType(code);
			if (fcInformationType)
			{
				CInformationCodeString cs;
				cs._name = code;
				cs._id = informationType->GetIDAsInteger();
				cs._ir = informationType;
				ngflist.push_back(cs);
			}
		}
	}
	
	sort(ngflist.begin(), ngflist.end());
	InitInformationFeatureList();
}

void CDialogViewInformationType::InitInformationFeatureList() 
{
	m_ViewListInformationType.DeleteAllItems();
	auto fc = ((S100Layer*)s100Dataset->m_pLayer)->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	int iItem = 0;
	for (auto fni = ngflist.begin(); fni != ngflist.end(); fni++)
	{
		CInformationCodeString* cs = &(*fni);
		LVITEM lvi;
		lvi.mask = /*LVIF_IMAGE | */LVIF_TEXT;
		lvi.iItem = iItem;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(cs->_name.c_str());
		lvi.lParam = (LPARAM)cs->_ir;
		BOOL ret = 0;

		CString id;
		id.Format(L"%d", cs->_id);
		int indexItem = m_ViewListInformationType.InsertItem(&lvi);

		// Search the feature catalog again to find the description of the information type.
		std::wstring csCnt;

		for (auto itor = fc->GetInformationTypes()->GetInformationTypePointer().begin();
			itor != fc->GetInformationTypes()->GetInformationTypePointer().end();
			itor++)
		{
			CString tmp;
			CString tmp1;
			
			tmp = itor->first.c_str();
			tmp1 = cs->_name.c_str();
		}
		std::wstring cnt;
		cnt = std::to_wstring(cs->_ir->GetInformationRelationCount());

		m_ViewListInformationType.SetItemText(indexItem, 0, id);
		m_ViewListInformationType.SetItemText(indexItem, 1, cs->_name.c_str());
		m_ViewListInformationType.SetItemText(indexItem, 2, cnt.c_str());

		iItem++;
	}
}
BEGIN_MESSAGE_MAP(CDialogViewInformationType, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogViewInformationType::OnBnClickedOk)
END_MESSAGE_MAP()

void CDialogViewInformationType::OnBnClickedOk()
{
	CString isCtrlClicked = L"1";
	CStringArray csa;
	csa.SetSize(10);

	POSITION pos = m_ViewListInformationType.GetFirstSelectedItemPosition();
	int idx = 0;

	if (pos != nullptr) //If there is a selected value,
	{
		while (pos) //There are cases where multiple is selected.
		{
			//This part needs to be modified: Only the value of the selected item must be exceeded.
			CString csFrid;
			CString csFeatureName;
			CString csCnt;

			idx = m_ViewListInformationType.GetNextSelectedItem(pos);
			csFrid.Format(m_ViewListInformationType.GetItemText(idx, 0));   // frid
			csFeatureName.Format(m_ViewListInformationType.GetItemText(idx, 1));   // featureName
			csCnt.Format(m_ViewListInformationType.GetItemText(idx,2)); //cnt
			csa.Add(isCtrlClicked + _T("|||") + csFrid + _T("|||") + csFrid + _T("|||") + L"-" + _T("|||") + L"-" + _T("|||") + L"-" + _T("|||") + csFeatureName + _T("|||") + L"0" + _T("|||") + L"Information");

			theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, s100Dataset, isCtrlClicked);
		}
	}
	CDialogEx::OnOK();
}