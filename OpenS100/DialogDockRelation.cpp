#include "pch.h"
#include "OpenS100.h"
#include "DialogDockRelation.h"

#include "../GISLibrary/S100Layer.h"
#include "../FeatureCatalog/FeatureCatalogue.h"

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
	m_ListRelation.InsertColumn(2, _T("Source Name"), LVCFMT_CENTER, (int)(listRect.Width()*0.25));
	m_ListRelation.InsertColumn(3, _T("Source ID"), LVCFMT_LEFT, (int)(listRect.Width() * 0));
	m_ListRelation.InsertColumn(4, _T("Role"), LVCFMT_CENTER, (int)(listRect.Width()*0.15));
	m_ListRelation.InsertColumn(5, _T("Target Name"), LVCFMT_LEFT, (int)(listRect.Width()*0.25));
	
	// hidden
	m_ListRelation.InsertColumn(6, _T("Target ID"), LVCFMT_LEFT, (int)(listRect.Width() * 0));
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

		theApp.m_DockablePaneEditWindow.m_cell = m_cell;
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

		theApp.m_DockablePaneEditWindow.m_cell = m_cell;
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

void CDialogDockRelation::SetFeatureList(S100SpatialObject* cell, std::list<GF::FeatureType*> flist, std::list<GF::InformationType*> infoList)
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
		auto srcFeature = *ri;

		// feature type code (source)
		std::wstring srcFeatureTypeCode = cell->GetFeatureTypeCodeByID(srcFeature->GetIDAsInteger());

		// Feature Type (FC)
		auto featureType = fc->GetFeatureType(srcFeatureTypeCode);
		if (featureType == nullptr)
		{
			OutputDebugString(L"Failed to find feature type code\n");
			continue;
		}

		// fasc
		int featureAssociationCount = srcFeature->GetFeatureRelationCount();
		for (int i = 0; i < featureAssociationCount; i++)
		{
			auto associatedFeatureTypeID = srcFeature->GetAssociatedFeatureID(i);
			auto associatedFeature = cell->GetFeatureType(associatedFeatureTypeID);
			auto associatedFeatureCode = cell->GetFeatureTypeCodeByID(std::stoi(associatedFeatureTypeID));
			auto featureAssociationCode = cell->GetFeatureAssociationCode(srcFeature, i);
			auto roleCode = cell->GetFeatureAssociationRoleCode(srcFeature,i );

			CString colSrcFeatureId = L"";
			CString colSrcFeatureName = L"";
			CString colAssociationName = L"";
			CString colAssociationRole = L"";
			CString colAssociationFeatureName = L"";
			CString colAssociationFeatureId = L"";

			auto fc_featureAssociation = fc->GetFeatureAssociation(pugi::as_wide(featureAssociationCode));
			auto fc_role = fc->GetRole(pugi::as_wide(roleCode));
			auto fc_associatedFeatureType = fc->GetFeatureType(associatedFeatureCode);

			if (featureType)
			{
				colSrcFeatureName.Format(_T("%s (%d)"), featureType->GetName().c_str(), srcFeature->GetIDAsInteger());
			}
			else
			{
				colSrcFeatureName.Format(_T("%s (%d)"), srcFeatureTypeCode, srcFeature->GetIDAsInteger());
			}

			if (fc_featureAssociation)
			{
				colAssociationName.Format(_T("%s"), fc_featureAssociation->GetName().c_str());
			}
			else
			{
				colAssociationName.Format(_T("%s"), pugi::as_wide(featureAssociationCode).c_str());
			}

			if (fc_role)
			{
				colAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			}
			else
			{
				colAssociationRole.Format(_T("%s"), pugi::as_wide(roleCode).c_str());
			}

			if (fc_associatedFeatureType)
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), fc_associatedFeatureType->GetName().c_str(), pugi::as_wide(associatedFeatureTypeID).c_str());
			}
			else
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), associatedFeatureCode, pugi::as_wide(associatedFeatureTypeID).c_str());
			}

			colSrcFeatureId.Format(_T("%d"), srcFeature->GetIDAsInteger());
			colAssociationFeatureId.Format(_T("%s"), pugi::as_wide(associatedFeatureTypeID).c_str());


			// set text
			m_ListRelation.InsertItem(0, L"Feature");
			m_ListRelation.SetItemText(0, 1, colAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colSrcFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colSrcFeatureId);  // Feature ID
			m_ListRelation.SetItemText(0, 4, colAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name			

			// hidden
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID

			UpdateData(false);
		}

		// inas
		int informationAssociationCount = srcFeature->GetInformationRelationCount();
		for (int i = 0; i < informationAssociationCount; i++)
		{
			auto associatedInformationTypeID = srcFeature->GetAssociatedInformationID(i);
			auto associatedInformationType = cell->GetInformationType(associatedInformationTypeID);
			auto associatedInformationTypeCode = cell->GetInformationTypeCodeByID(std::stoi(associatedInformationTypeID));
			auto informationAssociationCode = cell->GetInformationAssociationCode(srcFeature, i);
			auto roleCode = cell->GetInformationAssociationRoleCode(srcFeature, i);

			CString colSrcFeatureId = L"";
			CString colSrcFeatureName = L"";
			CString colAssociationName = L"";
			CString colAssociationRole = L"";
			CString colAssociationFeatureName = L"";
			CString colAssociationFeatureId = L"";

			auto fc_informationAssociation = fc->GetInformationAssociation(pugi::as_wide(informationAssociationCode));
			auto fc_role = fc->GetRole(pugi::as_wide(roleCode));
			auto fc_associatedInformationType = fc->GetInformationType(associatedInformationTypeCode);

			if (featureType)
			{
				colSrcFeatureName.Format(_T("%s (%d)"), featureType->GetName().c_str(), srcFeature->GetIDAsInteger());
			}
			else
			{
				colSrcFeatureName.Format(_T("%s (%d)"), srcFeatureTypeCode, srcFeature->GetIDAsInteger());
			}

			if (fc_informationAssociation)
			{
				colAssociationName.Format(_T("%s"), fc_informationAssociation->GetName().c_str());
			}
			else
			{
				colAssociationName.Format(_T("%s"), pugi::as_wide(informationAssociationCode).c_str());
			}

			if (fc_role)
			{
				colAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			}
			else
			{
				colAssociationRole.Format(_T("%s"), pugi::as_wide(roleCode).c_str());
			}

			if (fc_associatedInformationType)
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), fc_associatedInformationType->GetName().c_str(), pugi::as_wide(associatedInformationTypeID).c_str());
			}
			else
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), associatedInformationTypeCode, pugi::as_wide(associatedInformationTypeID).c_str());
			}

			colSrcFeatureId.Format(_T("%d"), srcFeature->GetIDAsInteger());
			colAssociationFeatureId.Format(_T("%s"), pugi::as_wide(associatedInformationTypeID).c_str());

			// set text
			m_ListRelation.InsertItem(0, L"Information");
			m_ListRelation.SetItemText(0, 1, colAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colSrcFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colSrcFeatureId);  // Feature ID
			m_ListRelation.SetItemText(0, 4, colAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name					
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID - hidden

			UpdateData(false);
		}
	}

	// Information, relatino is always about features.
	for (auto ri = infoList.begin(); ri != infoList.end(); ri++)
	{
		auto srcInformation = *ri;

		std::wstring srcInformationTypeCode = cell->GetInformationTypeCodeByID(srcInformation->GetIDAsInteger());

		auto informationType = fc->GetInformationType(srcInformationTypeCode);
		if (informationType == nullptr)
		{
			OutputDebugString(L"Failed to find infrrmation type code\n");
			continue;
		}

		// inas
		int informationAssociationCount = srcInformation->GetInformationRelationCount();
		for (int i = 0; i < informationAssociationCount; i++)
		{
			auto associatedInformationTypeID = srcInformation->GetAssociatedInformationID(i);
			auto associatedInformationType = cell->GetInformationType(associatedInformationTypeID);
			auto associatedInformationTypeCode = cell->GetInformationTypeCodeByID(std::stoi(associatedInformationTypeID));
			auto informationAssociationCode = cell->GetInformationAssociationCode(srcInformation, i);
			auto roleCode = cell->GetInformationAssociationRoleCode(srcInformation, i);

			CString colSrcFeatureId = L"";
			CString colSrcFeatureName = L"";
			CString colAssociationName = L"";
			CString colAssociationRole = L"";
			CString colAssociationFeatureName = L"";
			CString colAssociationFeatureId = L"";

			auto fc_informationAssociation = fc->GetInformationAssociation(pugi::as_wide(informationAssociationCode));
			auto fc_role = fc->GetRole(pugi::as_wide(roleCode));
			auto fc_associatedInformationType = fc->GetInformationType(associatedInformationTypeCode);

			if (informationType)
			{
				colSrcFeatureName.Format(_T("%s (%d)"), informationType->GetName().c_str(), srcInformation->GetIDAsInteger());
			}
			else
			{
				colSrcFeatureName.Format(_T("%s (%d)"), srcInformationTypeCode, srcInformation->GetIDAsInteger());
			}

			if (fc_informationAssociation)
			{
				colAssociationName.Format(_T("%s"), fc_informationAssociation->GetName().c_str());
			}
			else
			{
				colAssociationName.Format(_T("%s"), pugi::as_wide(informationAssociationCode).c_str());
			}

			if (fc_role)
			{
				colAssociationRole.Format(_T("%s"), fc_role->GetName().c_str());
			}
			else
			{
				colAssociationRole.Format(_T("%s"), pugi::as_wide(roleCode).c_str());
			}

			if (fc_associatedInformationType)
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), fc_associatedInformationType->GetName().c_str(), pugi::as_wide(associatedInformationTypeID).c_str());
			}
			else
			{
				colAssociationFeatureName.Format(_T("%s (%s)"), associatedInformationTypeCode, pugi::as_wide(associatedInformationTypeID).c_str());
			}

			colSrcFeatureId.Format(_T("%d"), srcInformation->GetIDAsInteger());
			colAssociationFeatureId.Format(_T("%s"), pugi::as_wide(associatedInformationTypeID).c_str());

			// set text
			m_ListRelation.InsertItem(0, L"Information");
			m_ListRelation.SetItemText(0, 1, colAssociationName); // Relation name
			m_ListRelation.SetItemText(0, 2, colSrcFeatureName);  // Feature name
			m_ListRelation.SetItemText(0, 3, colSrcFeatureId);  // Feature ID
			m_ListRelation.SetItemText(0, 4, colAssociationRole); // role
			m_ListRelation.SetItemText(0, 5, colAssociationFeatureName); // Association Feature name					
			m_ListRelation.SetItemText(0, 6, colAssociationFeatureId); // Feature ID - hidden

			UpdateData(false);
		}
	}
}