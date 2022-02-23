//

#include "pch.h"
#include "OpenS100.h"
#include "DialogViewNoGeometry.h"
#include "afxdialogex.h"
#include "resource.h"

#include "..\\GISLibrary\\CodeWithNumericCode.h"
#include "..\\GISLibrary\\S101Layer.h"
#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"

#include <string>
#include <algorithm>


IMPLEMENT_DYNAMIC(CDialogViewNoGeometry, CDialogEx)

BEGIN_MESSAGE_MAP(CDialogViewNoGeometry, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogViewNoGeometry::OnBnClickedOk)
END_MESSAGE_MAP()

CDialogViewNoGeometry::CDialogViewNoGeometry(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NOGEOMETRY, pParent)
{

}

CDialogViewNoGeometry::~CDialogViewNoGeometry()
{

}

void CDialogViewNoGeometry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NOGEOMETRY, m_ViewListNoGeometry);
}

void CDialogViewNoGeometry::SetNoGeometryFeatureList(S101Cell* cell)
{
	auto fc = ((S101Layer*)cell->m_pLayer)->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	POSITION pos = cell->GetFeatureStartPosition();
	m_cell = cell;
	while (pos != NULL)
	{
		__int64 key = 0;
		R_FeatureRecord* fr = NULL;
		cell->GetNextAssoc(pos, key, fr);

		if (fr->m_spas.size() == 0)
		{
			int code = fr->m_frid.m_nftc;

			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(code);
			CodeWithNumericCode* nc = itor->second;

			std::wstring codeStr = nc->m_code;
			FeatureType *objFT = &fc->GetFeatureTypesPointer().GetFeatureTypePointer().find(codeStr)->second;

			CFeatureCodeString cs;
			cs._name = objFT->GetCodeAsWString();
			cs._id = fr->m_frid.m_name.RCID;
			cs._fr = fr;
			ngflist.push_back(cs); 
		}
	}

	sort(ngflist.begin(), ngflist.end());
}

BOOL CDialogViewNoGeometry::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect listRect;
	m_ViewListNoGeometry.GetWindowRect(listRect);
	m_ViewListNoGeometry.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ViewListNoGeometry.InsertColumn(0, _T("ID"), LVCFMT_CENTER, (int)(listRect.Width()*0.20));
	m_ViewListNoGeometry.InsertColumn(1, _T("Name"), LVCFMT_LEFT, (int)(listRect.Width()*0.30));
	m_ViewListNoGeometry.InsertColumn(2, _T("Relation Cnt"), LVCFMT_LEFT, (int)(listRect.Width()*0.50));

	InitNonGeometryList();

	return true;
}

void CDialogViewNoGeometry::InitNonGeometryList()
{
	m_ViewListNoGeometry.DeleteAllItems();
	int iItem = 0;
	for (auto fni = ngflist.begin(); fni != ngflist.end(); fni++)
	{
		CFeatureCodeString* cs = &(*fni);
		LVITEM lvi;
		lvi.mask = /*LVIF_IMAGE | */LVIF_TEXT;
		lvi.iItem = iItem;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(cs->_name.c_str());
		lvi.lParam = (LPARAM)cs->_fr;
		BOOL ret = 0;

		CString id;
		id.Format(L"%d", cs->_id);
		int indexItem = m_ViewListNoGeometry.InsertItem(&lvi);
		std::wstring CntCount = std::to_wstring(cs->_fr->m_fasc.size() + cs->_fr->m_inas.size());

		m_ViewListNoGeometry.SetItemText(indexItem, 0, id);
		m_ViewListNoGeometry.SetItemText(indexItem, 1, cs->_name.c_str());
		m_ViewListNoGeometry.SetItemText(indexItem, 2, CntCount.c_str());

		iItem++;
	}

}

void CDialogViewNoGeometry::OnBnClickedOk()
{
	CStringArray csa;
	csa.SetSize(10);
	CString isCtrlClicked = L"1";
	POSITION pos = m_ViewListNoGeometry.GetFirstSelectedItemPosition();
	int idx = 0;

	if (pos != nullptr)
	{
		while (pos)
		{
			CString csFrid;
			CString csFeatureName;
			CString csCnt;
			idx = m_ViewListNoGeometry.GetNextSelectedItem(pos);
			csFrid.Format(m_ViewListNoGeometry.GetItemText(idx, 0));   // frid
			csFeatureName.Format(m_ViewListNoGeometry.GetItemText(idx, 1));   // featureName
			csCnt.Format(m_ViewListNoGeometry.GetItemText(idx, 2));

			csa.Add(isCtrlClicked + _T("|||") + csFrid + _T("|||") + csFrid + _T("|||") + L"-" + _T("|||") + L"-" + _T("|||") + L"999" + _T("|||") + csFeatureName + _T("|||") + csCnt + _T("|||") + L"Feature");

			theApp.m_DockablePaneCurrentSelection.UpdateListTest(&csa, m_cell, isCtrlClicked);
		}
	}
	CDialogEx::OnOK();
}