//
#include "pch.h"
#include "afxdialogex.h"
#include "DialogViewInformationType.h"
#include "resource.h"
#include "OpenS100.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\R_InformationRecord.h"
#include "..\\GISLibrary\\CodeWithNumericCode.h"
#include "..\\GISLibrary\\S101Layer.h"
#include "..\\FeatureCatalog\\InformationType.h"

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

	InitInformationFeatureList();

	return true;
}

void CDialogViewInformationType::SetInformationFeatureList(S101Cell* cell) 
{
	POSITION pos = cell->GetInfoStartPosition();
	auto fc = ((S101Layer*)cell->m_pLayer)->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	m_cell = cell;
	while (pos != NULL)
	{
		__int64 key = 0;
		R_InformationRecord* ir = NULL;
		cell->GetNextAssoc(pos, key, ir);

		int code = ir->m_irid.m_nitc;

		auto itor = cell->m_dsgir.m_itcs->m_arr.find(code);
		CodeWithNumericCode* nc = itor->second;

		std::wstring codeStr = nc->m_code;
		InformationType *objIT = &fc->GetInformationTypesPointer().GetInformationTypePointer().find(codeStr)->second;

		// Feature ID
		CInformationCodeString cs;
		cs._name = objIT->GetCodeAsWString();
		cs._id = ir->m_irid.m_name.RCID;
		cs._ir = ir;
		ngflist.push_back(cs);
	}
	sort(ngflist.begin(), ngflist.end());
}

void CDialogViewInformationType::InitInformationFeatureList() 
{
	auto fc = ((S101Layer*)m_cell->m_pLayer)->GetFeatureCatalog();
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

		for (auto itor = fc->GetInformationTypesPointer().GetInformationTypePointer().begin();
			itor != fc->GetInformationTypesPointer().GetInformationTypePointer().end();
			itor++)
		{
			CString tmp;
			CString tmp1;
			
			tmp = itor->first.c_str();
			tmp1 = cs->_name.c_str();
		}
		std::wstring cnt;
		cnt = std::to_wstring(cs->_ir->m_inas.size());

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

			theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, m_cell, isCtrlClicked);
		}
	}
	CDialogEx::OnOK();
}