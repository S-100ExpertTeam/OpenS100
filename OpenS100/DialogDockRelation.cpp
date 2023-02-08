#include "pch.h"
#include "OpenS100.h"
#include "DialogDockRelation.h"

#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\S100Layer.h"
#include "..\\GISLibrary\\F_FASC.h"
#include "..\\GISLibrary\\F_INAS.h"
#include "..\\GISLibrary\\R_InformationRecord.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\GISLibrary\\CodeWithNumericCode.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"

IMPLEMENT_DYNAMIC(CDialogDockRelation, CDialog)

CDialogDockRelation::CDialogDockRelation(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDockRelation::IDD, pParent)
{
	nSelectedItem = -1;
}

CDialogDockRelation::~CDialogDockRelation()
{

}

void CDialogDockRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RELATION, m_ListRelation);
}


BEGIN_MESSAGE_MAP(CDialogDockRelation, CDialog)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDOK, &CDialogDockRelation::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogDockRelation::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RELATION, &CDialogDockRelation::OnLvnItemchangedList)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(NM_CLICK, IDC_LIST_RELATION, &CDialogDockRelation::OnNMClickListLm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RELATION, &CDialogDockRelation::OnNMDblclkListLm)
END_MESSAGE_MAP()


// CDialogDockRelation message handlers
int CDialogDockRelation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CDialogDockRelation::OnBnClickedOk()
{
	return;
}

void CDialogDockRelation::OnBnClickedCancel()
{
	return;
}

void CDialogDockRelation::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CDialogDockRelation::OnSize(UINT nType, int cx, int cy)
{
	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectENCs;
	if (m_ListRelation.GetSafeHwnd())
	{
		rectENCs = CRect(rectClient.left, rectClient.top, rectClient.Size().cx, rectClient.Size().cy);
		m_ListRelation.MoveWindow(rectENCs);
		for (int i = 0; i < m_ListRelation.GetHeaderCtrl()->GetItemCount(); ++i)
		{
			m_ListRelation.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		}
	}
	CDialog::OnSize(nType, cx, cy);
}

void CDialogDockRelation::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
}

// layerManager
BOOL CDialogDockRelation::OnInitDialog()
{
	CDialog::OnInitDialog();
 	m_ListRelation.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListRelation.ModifyStyle(0, LVS_SHOWSELALWAYS);

	CRect listRect;
	m_ListRelation.GetWindowRect(listRect);

	m_ListRelation.InsertColumn(0, _T("Type"), LVCFMT_LEFT, (int)(listRect.Width()*0.1));
	m_ListRelation.InsertColumn(1, _T("Relation name"), LVCFMT_LEFT, (int)(listRect.Width()*0.2));
	m_ListRelation.InsertColumn(2, _T("Feature Name"), LVCFMT_CENTER, (int)(listRect.Width()*0.25));
	m_ListRelation.InsertColumn(3, _T("Feature ID"), LVCFMT_LEFT, (int)(listRect.Width() * 0));
	m_ListRelation.InsertColumn(4, _T("Role"), LVCFMT_CENTER, (int)(listRect.Width()*0.15));
	m_ListRelation.InsertColumn(5, _T("Association Feature Name"), LVCFMT_LEFT, (int)(listRect.Width()*0.25));

	// hidden
	m_ListRelation.InsertColumn(6, _T("Association Feature ID"), LVCFMT_LEFT, (int)(listRect.Width() * 0));
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogDockRelation::UpdateList()
{
	m_ListRelation.DeleteAllItems();
}

void CDialogDockRelation::OnNMClickListLm(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_ListRelation.GetFirstSelectedItemPosition();
	int idx = 0;

	// Delete all inverted items

	idx = m_ListRelation.GetNextSelectedItem(pos);
	CString type = m_ListRelation.GetItemText(idx, 0);

	if (type == L"Information")
	{
		CString frid;
		frid.Format(m_ListRelation.GetItemText(idx, 5));
		int num=frid.FindOneOf(L"("); //Delete the string to "("
		frid.Delete(0, num);
		frid.Replace(L"(", L"");
		frid.Replace(L")", L"");


		__int64 fridN = _tcstoui64(frid, NULL, 10);
		__int64 key = ((__int64)150) << 32 | fridN;
		auto stringKey = std::to_wstring(key);

		auto infmap = m_cell->GetInformationRecord(key);
		if (infmap == nullptr)
		{
			return;
		}

		R_InformationRecord *inr = m_cell->GetInformationRecord(key);
		S101Cell* cell = m_cell;

		auto itor = cell->m_dsgir.m_ftcs->m_arr.find(inr->m_irid.NITC());
		if (itor == cell->m_dsgir.m_ftcs->m_arr.end()) {
			return;
		}

		auto fc = ((S100Layer*)cell->m_pLayer)->GetFeatureCatalog();
		if (nullptr == fc)
		{
			return;
		}
		
		FeatureType* ft = fc->GetFeatureType(itor->second->m_code.GetBuffer());

		theApp.m_DockablePaneEditWindow.m_cell = cell;
		theApp.m_DockablePaneEditWindow.SetInformationType(stringKey);
	}
	else if (type == L"Feature")
	{
		CString frid;
		frid.Format(m_ListRelation.GetItemText(idx, 5));
		int num = frid.FindOneOf(L"("); //Delete the string to "("
		frid.Delete(0, num);
		frid.Replace(L"(", L"");
		frid.Replace(L")", L"");

		__int64 fridN = _tcstoui64(frid, NULL, 10);
		__int64 key = ((__int64)100) << 32 | fridN; // feature
		auto stringKey = std::to_wstring(key);

		auto feumap = m_cell->GetFeatureRecord(key);
		R_FeatureRecord *rfr = feumap;
		if (feumap == nullptr)
		{
			return;
		}

		R_FeatureRecord *pFe = rfr;
		S101Cell* cell = m_cell;

		auto itor = cell->m_dsgir.m_ftcs->m_arr.find(pFe->m_frid.m_nftc);
		auto fc = ((S100Layer*)cell->m_pLayer)->GetFeatureCatalog();
		if (nullptr == fc)
		{
			return;
		}

		FeatureType* ft = fc->GetFeatureType(itor->second->m_code.GetBuffer());
		theApp.m_DockablePaneEditWindow.m_cell = cell;
		theApp.m_DockablePaneEditWindow.SetFeatureType(stringKey);

	}
}

void CDialogDockRelation::OnNMDblclkListLm(NMHDR *pNMHDR, LRESULT *pResult)
{

	POSITION pos = m_ListRelation.GetFirstSelectedItemPosition();
	int idx = 0;
	idx = m_ListRelation.GetNextSelectedItem(pos);

	CString type;

	type.Format(m_ListRelation.GetItemText(idx, 0));
	if (type == "Feature")
	{
	}
	else if (type == "Information")
	{
	}
	// Even if I choose both, something hasn't changed.
}

void CDialogDockRelation::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	CRect      rectENCs;

	if (m_ListRelation.GetSafeHwnd())
	{
		rectENCs = CRect(rectClient.left, rectClient.top, rectClient.Size().cx, rectClient.Size().cy);
		m_ListRelation.MoveWindow(rectENCs);
		m_ListRelation.SetColumnWidth(0, (int)(rectENCs.Width()*0.1));						// Adjusting the area of column 0.
		m_ListRelation.SetColumnWidth(1, (int)(rectENCs.Width()*0.2));						// Adjusting the area of column 1.
		m_ListRelation.SetColumnWidth(2, (int)(rectENCs.Width()*0.25));						// Adjusting the area of column 2.
		m_ListRelation.SetColumnWidth(3, (int)(rectENCs.Width() * 0));						// Adjusting the area of column 3.
		m_ListRelation.SetColumnWidth(4, (int)(rectENCs.Width()*0.15));						// Adjusting the area of column 4.
		m_ListRelation.SetColumnWidth(5, (int)(rectENCs.Width()*0.25));						// Adjusting the area of column 5.
		m_ListRelation.SetColumnWidth(6, (int)(rectENCs.Width() * 0));						// Adjusting the area of column 6.
	}
}

void CDialogDockRelation::SetFeatureList(S101Cell* cell, std::list<R_FeatureRecord*> flist, std::list<R_InformationRecord*> infoList)
{
	m_cell = cell;
	// Erase everything before insertion.
	m_ListRelation.DeleteAllItems();

	auto fc = ((S100Layer*)cell->m_pLayer)->GetFeatureCatalog();
	if (nullptr == fc)
	{
		return;
	}

	for (auto ri = flist.begin(); ri != flist.end(); ri++)
	{
		R_FeatureRecord *orgFr = *ri;	// Feature that needs a relation

		// feature type code (source)
		std::wstring orgFeatureTypeCode = cell->m_dsgir.GetFeatureCode(orgFr->GetNumericCode());

		// Feature Type (FC)
		auto featureType = fc->GetFeatureType(orgFeatureTypeCode);
		if (featureType == nullptr)
		{
			OutputDebugString(L"Failed to find feature type code\n");
			continue;
		}

		// fasc
		int count = (int)orgFr->m_fasc.size();
		for (auto itt = orgFr->m_fasc.begin(); itt != orgFr->m_fasc.end(); itt++)
		{
			F_FASC *fasc = *itt;
			auto featureAssociationCode = cell->m_dsgir.GetFeatureAssociationCode(fasc->m_nfac);
			if (featureAssociationCode.IsEmpty())
			{
				continue;
			}

			auto roleCode = cell->m_dsgir.GetAssociationRoleCode(fasc->m_narc);
			if (roleCode.IsEmpty())
			{
				continue;
			}

			//auto orgFBItor = featureType->GetFeatureBindingPointer().find(orgFeatureTypeCode);
			std::wstring orgAssociationName = featureAssociationCode;
			std::wstring orgRoleName = roleCode;

			LVITEM firstItem = { 0 };
			m_ListRelation.InsertItem(&firstItem);  // insert item

			CString colOrgFeatureId = L"";
			CString colOrgFeatureName = L"";
			CString colAssociationFeatureId = L"";
			CString colAssociationFeatureName = L"";
			CString colOrgAssociationName = L"";
			CString colOrgAssociationRole = L"";

			int n_code = orgFr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(n_code);
			auto ws_objectCode1 = itor->second->m_code.GetBuffer();
			itor->second->m_code.ReleaseBuffer();
			auto fc_featureTypeORG = fc->GetFeatureType(ws_objectCode1);
			auto fc_featureAssociation = fc->GetFeatureAssociation(orgAssociationName);
			auto fc_role = fc->GetRole(orgRoleName);

			__int64 key = ((__int64)100) << 32 | fasc->m_name.RCID;

			auto associatedFeature = m_cell->GetFeatureRecord(key);
			if (associatedFeature == nullptr)
			{
				CString msg;
				msg.Format(L"[%s] Feature not found. -REALATION", featureAssociationCode);
				int i = 0;
				continue;
			}

			R_FeatureRecord* rfr = m_cell->GetFeatureRecord(key);
			auto itors = cell->m_dsgir.m_ftcs->m_arr.find(rfr->m_frid.m_nftc);
			auto ws_objectCode2 = itors->second->m_code.GetBuffer();
			itors->second->m_code.ReleaseBuffer();
			auto fc_informationTypeOBJ = fc->GetFeatureType(ws_objectCode2);

			if (fc_featureTypeORG)
			{
				colOrgFeatureName.Format(_T("%s (%d)"), fc_featureTypeORG->GetName().c_str(), orgFr->m_frid.m_name.RCID);
			}
			else
			{
				colOrgFeatureName.Format(_T("%s (%d)"), ws_objectCode1, orgFr->m_frid.m_name.RCID);
			}

			if (fc_featureAssociation)
			{
				colOrgAssociationName.Format(_T("%s"), fc_featureAssociation->GetName().c_str());
			}
			else
			{
				colOrgAssociationName.Format(_T("%s"), orgAssociationName);
			}

			if (fc_role)
			{
				colOrgAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			}
			else
			{
				colOrgAssociationRole.Format(_T("%s"), orgRoleName.c_str());
			}

			if (fc_informationTypeOBJ)
			{
				colAssociationFeatureName.Format(_T("%s (%d)"), fc_informationTypeOBJ->GetName().c_str(), fasc->m_name.RCID);
			}
			else
			{
				colAssociationFeatureName.Format(_T("%s (%d)"), ws_objectCode2, fasc->m_name.RCID);
			}

			colOrgFeatureId.Format(_T("%d"), orgFr->m_frid.m_name.RCID);
			colAssociationFeatureId.Format(_T("%d"), fasc->m_name.RCID);


			// set text
			m_ListRelation.SetItemText(0, 0, L"Feature");
			m_ListRelation.SetItemText(0, 1, colOrgAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colOrgFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colOrgFeatureId);  // Feature ID
			m_ListRelation.SetItemText(0, 4, colOrgAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name			

			// hidden
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID

			UpdateData(false);
		}

		// inas
		for (auto itt = orgFr->m_inas.begin(); itt != orgFr->m_inas.end(); itt++)
		{
			F_INAS *inas = *itt;

			auto orgfaItor = cell->m_dsgir.m_iacs->m_arr.find(inas->m_niac);
			if (orgfaItor == cell->m_dsgir.m_iacs->m_arr.end())  continue;
			CString faName = orgfaItor->second->m_code;

			auto orgarItor = cell->m_dsgir.m_arcs->m_arr.find(inas->m_narc);
			if (orgarItor == cell->m_dsgir.m_arcs->m_arr.end())  continue;
			CString arName = orgarItor->second->m_code;

			std::wstring orgAssociationName = faName;
			std::wstring orgRoleName = arName;

			LVITEM firstItem = { 0 };
			m_ListRelation.InsertItem(&firstItem);  // insert item

			CString colOrgFeatureId = L"";
			CString colOrgFeatureName = L"";
			CString colAssociationFeatureId = L"";
			CString colAssociationFeatureName = L"";
			CString colOrgAssociationName = L"";
			CString colOrgAssociationRole = L"";


			////////////////////////////////////////////////////////////////////////////////
			int n_code = orgFr->m_frid.m_nftc;
			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(n_code);
			auto ws_objectCode1 = itor->second->m_code.GetBuffer();
			itor->second->m_code.ReleaseBuffer();
			auto fc_featureType = fc->GetFeatureType(ws_objectCode1);
			auto fc_featureAssociation = fc->GetInformationAssociation(orgAssociationName);
			auto fc_role = fc->GetRole(orgRoleName);

			__int64 key = ((__int64)150) << 32 | inas->m_name.RCID;
			R_InformationRecord* rfr = m_cell->GetInformationRecord(key);
			//R_InformationRecord* rfr = m_cell->m_infMap.PLookup(key)->value;
			auto itors = cell->m_dsgir.m_itcs->m_arr.find(rfr->m_irid.NITC());
			auto ws_objectCode2 = itors->second->m_code.GetBuffer();
			itors->second->m_code.ReleaseBuffer();
			auto fc_informationType = fc->GetInformationType(ws_objectCode2);
			////////////////////////////////////////////////////////////////////////////////

			if (fc_featureType)
				colOrgFeatureName.Format(_T("%s (%d)"), fc_featureType->GetName().c_str(), orgFr->m_frid.m_name.RCID);
			else
				colOrgFeatureName.Format(_T("%s (%d)"), ws_objectCode1, orgFr->m_frid.m_name.RCID);
			if (fc_featureAssociation)
				colOrgAssociationName.Format(_T("%s"), fc_featureAssociation->GetName().c_str());
			else
				colOrgAssociationName.Format(_T("%s"), orgAssociationName);
			if (fc_role)
				colOrgAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			else
				colOrgAssociationRole.Format(_T("%s"), orgRoleName.c_str());
			if (fc_informationType)
				colAssociationFeatureName.Format(_T("%s (%d)"), fc_informationType->GetName().c_str(), inas->m_name.RCID);
			else
				colAssociationFeatureName.Format(_T("%s (%d)"), ws_objectCode2, inas->m_name.RCID);


			colOrgFeatureId.Format(_T("%d"), orgFr->m_frid.m_name.RCID);

			colAssociationFeatureId.Format(_T("%d"), inas->m_name.RCID);

			// set text
			m_ListRelation.SetItemText(0, 0, L"Information");

			m_ListRelation.SetItemText(0, 1, colOrgAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colOrgFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colOrgFeatureId);  // Feature ID - hidden
			m_ListRelation.SetItemText(0, 4, colOrgAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name			
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID - hidden

			UpdateData(false);
		}
	}

	// Information, relatino is always about features.
	for (auto ri = infoList.begin(); ri != infoList.end(); ri++)
	{
		R_InformationRecord *orgIr = *ri;	// Feature that needs a relation

		auto orgItItor = cell->m_dsgir.m_itcs->m_arr.find(orgIr->m_irid.NITC());
		if (orgItItor == cell->m_dsgir.m_itcs->m_arr.end())  continue;
		std::wstring orgFeatureName = orgItItor->second->m_code;

		InformationType *orgIt = fc->GetInformationTypes()->GetInformationTypePointer().find(orgFeatureName)->second;

		// fasc
		for (auto itt = orgIr->m_inas.begin(); itt != orgIr->m_inas.end(); itt++)
		{
			F_INAS *inas = *itt;

			auto orgfaItor = cell->m_dsgir.m_iacs->m_arr.find(inas->m_niac);
			if (orgfaItor == cell->m_dsgir.m_iacs->m_arr.end())  continue;
			CString faName = orgfaItor->second->m_code;

			auto orgarItor = cell->m_dsgir.m_arcs->m_arr.find(inas->m_narc);
			if (orgarItor == cell->m_dsgir.m_arcs->m_arr.end())  continue;
			CString arName = orgarItor->second->m_code;

			std::wstring orgAssociationName = faName;
			std::wstring orgRoleName = arName;

			LVITEM firstItem = { 0 };
			m_ListRelation.InsertItem(&firstItem);  // insert item

			CString colOrgFeatureId = L"";
			CString colOrgFeatureName = L"";
			CString colAssociationFeatureId = L"";
			CString colAssociationFeatureName = L"";
			CString colOrgAssociationName = L"";
			CString colOrgAssociationRole = L"";


			////////////////////////////////////////////////////////////////////////////////
			int n_code = orgIr->m_irid.NITC();
			auto itor = cell->m_dsgir.m_itcs->m_arr.find(n_code);
			auto ws_objectCode1 = itor->second->m_code.GetBuffer();
			itor->second->m_code.ReleaseBuffer();
			auto fc_informationTypeORG = fc->GetInformationType(ws_objectCode1);
			auto fc_featureAssociation = fc->GetInformationAssociation(orgAssociationName);
			auto fc_role = fc->GetRole(orgRoleName);

			__int64 key = ((__int64)150) << 32 | inas->m_name.RCID;

			R_InformationRecord* rfr = m_cell->GetInformationRecord(key);

			auto itors = cell->m_dsgir.m_itcs->m_arr.find(rfr->m_irid.NITC());
			auto ws_objectCode2 = itors->second->m_code.GetBuffer();
			itors->second->m_code.ReleaseBuffer();
			auto fc_informationTypeOBJ = fc->GetInformationType(ws_objectCode2);
			////////////////////////////////////////////////////////////////////////////////

			if (fc_informationTypeORG)
				colOrgFeatureName.Format(_T("%s (%d)"), fc_informationTypeORG->GetName().c_str(), orgIr->m_irid.m_name.RCID);
			else
				colOrgFeatureName.Format(_T("%s (%d)"), ws_objectCode1, orgIr->m_irid.m_name.RCID);
			if (fc_featureAssociation)
				colOrgAssociationName.Format(_T("%s"), fc_featureAssociation->GetName().c_str());
			else
				colOrgAssociationName.Format(_T("%s"), orgAssociationName);
			if (fc_role)
				colOrgAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			else
				colOrgAssociationRole.Format(_T("%s"), orgRoleName.c_str());
			if (fc_informationTypeOBJ)
				colAssociationFeatureName.Format(_T("%s (%d)"), fc_informationTypeOBJ->GetName().c_str(), inas->m_name.RCID);
			else
				colAssociationFeatureName.Format(_T("%s (%d)"), ws_objectCode2, inas->m_name.RCID);

			colOrgFeatureId.Format(_T("%d"), orgIr->m_irid.m_name.RCID);
			colAssociationFeatureId.Format(_T("%d"), inas->m_name.RCID);

			// set text
			m_ListRelation.SetItemText(0, 0, L"Information");

			m_ListRelation.SetItemText(0, 1, colOrgAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colOrgFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colOrgFeatureId);  // Feature ID - hidden
			m_ListRelation.SetItemText(0, 4, colOrgAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name			
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID - hidden

			UpdateData(false);
		}
	}
}