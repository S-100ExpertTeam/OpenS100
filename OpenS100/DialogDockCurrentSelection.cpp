#include "pch.h"
#include "DialogDockCurrentSelection.h"
#include "DialogDockRelation.h"
#include "OpenS100.h"
#include "OpenS100View.h"

#include <vector>

#include "..\\GISLibrary\\S101Layer.h"
#include "..\\GISLibrary\\F_FASC.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\GISLibrary\\R_InformationRecord.h"
#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\F_INAS.h"
#include "..\\GISLibrary\\CodeWithNumericCode.h"
#include "..\\GISLibrary\\GISLibrary.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\FeatureCatalog\\FeatureType.h"


IMPLEMENT_DYNAMIC(CDialogDockCurrentSelection, CDialogEx)

CDialogDockCurrentSelection::CDialogDockCurrentSelection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogDockCurrentSelection::IDD, pParent)
{

}

CDialogDockCurrentSelection::~CDialogDockCurrentSelection()
{

}

void CDialogDockCurrentSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CURRENT_SELECTION, m_ListCurrentSelection);
}

BEGIN_MESSAGE_MAP(CDialogDockCurrentSelection, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CURRENT_SELECTION, &CDialogDockCurrentSelection::OnLvnItemchangedList)

END_MESSAGE_MAP()

// CDialogDockCurrentSelection message handlers
int CDialogDockCurrentSelection::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

//Current Selection item has been selected.
void CDialogDockCurrentSelection::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uChanged & LVIF_STATE)
	{
		if (pNMLV->uNewState & (LVIS_SELECTED | LVIS_FOCUSED))
		{
			if (pNMLV->iItem < 0)
			{
				return;
			}

			S101Cell* cell = m_Cell;
			FeatureCatalogue* fc = ((S101Layer*)cell->m_pLayer)->GetFeatureCatalog();
			if (nullptr == fc)
			{
				return;
			}

			POSITION pos = m_ListCurrentSelection.GetFirstSelectedItemPosition();
			int idx = 0;

			// Delete all inverted items.
			CString objId;
			CString featureType;

			idx = m_ListCurrentSelection.GetNextSelectedItem(pos);
			if (idx < 0)
			{
				return;
			}

			// RCID 
			objId.Format(m_ListCurrentSelection.GetItemText(idx, 1));

			// Feature or Information
			featureType.Format(m_ListCurrentSelection.GetItemText(idx, 7));
			__int64 objIdN = _tcstoui64(objId, NULL, 10);

			if (featureType == L"Feature")
			{
				__int64 key = ((__int64)100) << 32 | objIdN;

				R_FeatureRecord *rfr = m_Cell->GetFeatureRecord(key);
				R_FeatureRecord *pFe = rfr;
				
				if (cell == nullptr)
				{
					return;
				}

				auto itor = cell->m_dsgir.m_ftcs->m_arr.find(pFe->m_frid.m_nftc);

				// feature type
				FeatureType* ft = fc->GetFeatureType(itor->second->m_code.GetBuffer());
				CString geoType = m_ListCurrentSelection.GetItemText(idx, 3);

				if (geoType != L"No geometry")
				{
					auto layer = cell->GetLayer();
					if (nullptr != layer)
					{
						if (true == layer->IsOn())
						{
							theApp.pView->SetPickReportFeature(pFe);
							theApp.pView->Invalidate(FALSE);
						}
					}
				}

				theApp.m_DockablePaneEditWindow.SetSpatialObject(cell);
				theApp.m_DockablePaneEditWindow.SetFeatureRecord(pFe);
			}
			else if (featureType == L"Information")
			{
				__int64 key = ((__int64)150) << 32 | objIdN;

				if (m_Cell->GetInfoMapCount() == 0)
				{
					int i = 0;
					return;
				}

				R_InformationRecord *rfr = m_Cell->GetInformationRecord(key);
				S101Cell* cell = m_Cell;

				auto itor = cell->m_dsgir.m_itcs->m_arr.find(rfr->m_irid.NITC());

				m_selectedInformationType = fc->GetInformationType(std::wstring(itor->second->m_code));

				theApp.m_DockablePaneEditWindow.SetSpatialObject(cell);
				theApp.m_DockablePaneEditWindow.SetFeatureRecord(rfr);
			}

			pos = m_ListCurrentSelection.GetFirstSelectedItemPosition();
			idx = 0;

			std::list<R_FeatureRecord*> flist;
			std::list<R_InformationRecord*> infoList;

			while (pos)
			{
				CString objId;
				CString featureType;
				idx = m_ListCurrentSelection.GetNextSelectedItem(pos);
				objId.Format(m_ListCurrentSelection.GetItemText(idx, 1));
				featureType.Format(m_ListCurrentSelection.GetItemText(idx, 7)); // feature/information type
				__int64 objIdN = _tcstoui64(objId, NULL, 10);
				R_FeatureRecord *rfr;
				R_InformationRecord *ifr;

				if (featureType == L"Feature")
				{
					__int64 key = ((__int64)100) << 32 | objIdN;
					rfr = m_Cell->GetFeatureRecord(key);
					flist.push_back(rfr);
				}
				else if (featureType == L"Information")
				{
					__int64 key = ((__int64)150) << 32 | objIdN;
					ifr = m_Cell->GetInformationRecord(key);
					infoList.push_back(ifr);
				}
			}
			theApp.m_DockablePaneRelation.pDlg->SetFeatureList(m_Cell, flist, infoList);
		}
	}
	*pResult = 0;
}

void CDialogDockCurrentSelection::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CDialogDockCurrentSelection::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
}

//// current selection
BOOL CDialogDockCurrentSelection::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*
	** init List Ctrl
	*/
	m_ListCurrentSelection.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListCurrentSelection.ModifyStyle(NULL, LVS_SHOWSELALWAYS); // multi select 

	CRect listRect;
	m_ListCurrentSelection.GetWindowRect(listRect);
	m_ListCurrentSelection.InsertColumn(0, _T("Feature ID "), LVCFMT_LEFT, (int)(listRect.Width()*0.10));
	m_ListCurrentSelection.InsertColumn(1, _T("FrID "), LVCFMT_LEFT, (int)(listRect.Width() * 0));  // invisible
	m_ListCurrentSelection.InsertColumn(2, _T("Name"), LVCFMT_LEFT, (int)(listRect.Width()*0.30));
	m_ListCurrentSelection.InsertColumn(3, _T("Geometry"), LVCFMT_LEFT, (int)(listRect.Width()*0.10));
	m_ListCurrentSelection.InsertColumn(4, _T("Lat"), LVCFMT_CENTER, (int)(listRect.Width()*0.15));
	m_ListCurrentSelection.InsertColumn(5, _T("Lon"), LVCFMT_CENTER, (int)(listRect.Width()*0.15));
	m_ListCurrentSelection.InsertColumn(6, _T("Relation Cnt"), LVCFMT_LEFT, (int)(listRect.Width()*0.10));
	m_ListCurrentSelection.InsertColumn(7, _T("Feature type"), LVCFMT_LEFT, (int)(listRect.Width()*0.10));

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogDockCurrentSelection::UpdateListTest(CStringArray *csa, S101Cell *cell, CString isCtrlClicked)
{
	CString tmp;
	m_Cell = cell;

	// If ctrl is not pressed, delete the item and start;
	if (isCtrlClicked == L"0")
	{
		m_ListCurrentSelection.DeleteAllItems();
	}
	
	auto fc = theApp.gisLib->GetFC();
	
	if (nullptr == fc)
	{
		return;
	}

	for (int i = 0; i < csa->GetSize(); i++)
	{
		int nTokenPos = 0;
		int cnt = 0;

		CString strToken = csa->GetAt(i).Tokenize(_T("|||"), nTokenPos);

		if (strToken != L"")
		{
			CString cs = csa->GetAt(i);
			std::string str1 = std::string(CT2CA(cs.operator LPCWSTR()));

			char toks[4] = { "|||" };
			std::vector<std::string> tok;
			StringSplit(str1, toks, tok);

			CString foid = CString::CStringT(CA2CT(tok[1].c_str()));
			CString frid = CString::CStringT(CA2CT(tok[2].c_str()));
			CString lat = CString::CStringT(CA2CT(tok[3].c_str()));
			CString lon = CString::CStringT(CA2CT(tok[4].c_str()));
			CString csType = CString::CStringT(CA2CT(tok[5].c_str()));
			std::wstring ws_name = std::wstring(tok[6].begin(), tok[6].end());
			CString name;
			CString assoCnt = CString::CStringT(CA2CT(tok[7].c_str()));
		
			CString featureType = CString::CStringT(CA2CT(tok[8].c_str()));
			CString type;

			if (featureType == L"Feature") 
			{
				auto fc_featureTypeORG = fc->GetFeatureType(ws_name);
				
				if (fc_featureTypeORG)
					name.Format(_T("%s"), fc_featureTypeORG->GetName().c_str());
				else
					name.Format(_T("%s"), ws_name);
			}
			else 
			{
				auto fc_inforTypeORG = fc->GetInformationType(ws_name);
				if (fc_inforTypeORG)
					name.Format(_T("%s"), fc_inforTypeORG->GetName().c_str());
				else
					name.Format(_T("%s"), ws_name);
			}

			if (csType == "1") {
				type = "Point";
			}
			else if (csType == "2" || csType == "5") {
				type = "Curve";
			}
			else if (csType == "3") {
				type = "Surface";
			}
			else if (csType == "4") {
				type = "Multi Point";
			}
			else if (csType == "999") {
				type = "No Geometry";
			}
			else {
				type = "-";
			}

			bool isExist = false;

			// duplicate check (frid)
			for (int j = 0; j < m_ListCurrentSelection.GetItemCount(); j++)
			{
				if (frid == m_ListCurrentSelection.GetItemText(j, 0) && featureType == m_ListCurrentSelection.GetItemText(j, 7))
				{
					isExist = true;
				}
			}

			int count = m_ListCurrentSelection.GetItemCount();

			if (!isExist)
			{
				LVITEM firstItem = { 0 };
				m_ListCurrentSelection.InsertItem(&firstItem);  // insert item
				m_ListCurrentSelection.SetFocus();
		
				// set text
				m_ListCurrentSelection.SetItemText(0, 0, frid);
				m_ListCurrentSelection.SetItemText(0, 1, frid);
				m_ListCurrentSelection.SetItemText(0, 2, name);
				m_ListCurrentSelection.SetItemText(0, 3, type);
				m_ListCurrentSelection.SetItemText(0, 4, lon);
				m_ListCurrentSelection.SetItemText(0, 5, lat);
				m_ListCurrentSelection.SetItemText(0, 6, assoCnt);
				m_ListCurrentSelection.SetItemText(0, 7, featureType);

				if (featureType == L"Feature")
				{
					// Acquired a catalog.
					auto it = fc->GetFeatureTypes().GetFeatureType().find(ws_name);
					if (it == fc->GetFeatureTypes().GetFeatureType().end())
					{
						CString msg;
						msg.Format(L"[%s] Feature not found. -CURRENT SELECTION", name.GetBuffer());
					}
					else
					{
						FeatureType* ft = &it->second;
						m_ListCurrentSelection.SetItemData(0, (LPARAM)ft);
					}
				}

				UpdateData(false);
			}
		}
	}

	int nOldItem = m_ListCurrentSelection.GetSelectionMark(); //Release the existing Current Selection value.
	if (nOldItem>=0)
	{
		m_ListCurrentSelection.SetItemState(nOldItem, 0, LVIS_SELECTED| LVIS_FOCUSED);
	}

	m_ListCurrentSelection.SetSelectionMark(0);
	m_ListCurrentSelection.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	theApp.pView->SetFocus();
}

void CDialogDockCurrentSelection::DeleteItem(CString id)
{
	int count = m_ListCurrentSelection.GetItemCount();
	if (count == 0)
	{
		return;
	}

	for (int i = 0; i < count; i++)
	{
		auto itemid = m_ListCurrentSelection.GetItemText(i, 0);
		if (itemid == id)
		{
			m_ListCurrentSelection.DeleteItem(i);
			break;
		}
	}

	theApp.m_DockablePaneEditWindow.DeleteAllItems(); //delete all feature information list 
}

void CDialogDockCurrentSelection::StringSplit(
	std::string strTarget,
	std::string strTok, std::vector<std::string> &strResult)
{
	int     nCutPos;
	int     nIndex = 0;
	
	while ((nCutPos = (int)strTarget.find_first_of(strTok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			strResult.push_back(strTarget.substr(0, nCutPos));
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		strResult.push_back(strTarget.substr(0, nCutPos));
	}
}


void CDialogDockCurrentSelection::UpdateList()
{
	int i = 0;
	POSITION pos = m_ListCurrentSelection.GetFirstSelectedItemPosition();
	int idx = 0;

	CString objId;
	CString featureType;


	while (idx >= 0)
	{
		idx = m_ListCurrentSelection.GetNextSelectedItem(pos);

		objId.Format(m_ListCurrentSelection.GetItemText(idx, 1));
		featureType.Format(m_ListCurrentSelection.GetItemText(idx, 7));
		__int64 objIdN = _tcstoui64(objId, NULL, 10);

		if (featureType == L"Feature")
		{
			__int64 key = ((__int64)100) << 32 | objIdN;
			R_FeatureRecord *rfr = m_Cell->GetFeatureRecord(key);

			int count = 0;

			int inforCount = (int)rfr->m_inas.size();
			count += inforCount;
			if (0 < inforCount)
			{
				auto inas = *rfr->m_inas.begin();
				int arrCount = (int)inas->m_arr.size();
				count = count + arrCount;
			}


			int feaCount = (int)rfr->m_fasc.size();
			count += feaCount;

			if (0 < feaCount)
			{
				auto fea = *rfr->m_fasc.begin();
				int arrCount = (int)fea->m_arr.size();
				count = count + arrCount;
			}

			CString Countstring;
			Countstring.Format(_T("%d"), count);
			m_ListCurrentSelection.SetItemText(idx, 6, Countstring);
		}
		else if (featureType == L"Information")
		{

			__int64 key = ((__int64)150) << 32 | objIdN;

			if (m_Cell->GetInfoMapCount() == 0)
			{
				int i = 0;
				return;
			}

			R_InformationRecord *rfr = m_Cell->GetInformationRecord(key);

			int count = 0;
			int inforCount = (int)rfr->m_inas.size();
			count = count + inforCount;

			if (0 < inforCount)
			{
				auto inas = *rfr->m_inas.begin();
				int arrCount = (int)inas->m_arr.size();
				count = count + arrCount;
			}

			CString Countstring;
			Countstring.Format(_T("%d"), inforCount);
			m_ListCurrentSelection.SetItemText(idx, 6, Countstring);
		}
	}
}

void CDialogDockCurrentSelection::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	CRect rectCombo;
	CRect rectENCs;

	GetClientRect(rectClient);

	if (m_ListCurrentSelection.GetSafeHwnd())
	{
		rectENCs = CRect(rectClient.left, rectClient.top, rectClient.Size().cx, rectClient.Size().cy);
		m_ListCurrentSelection.MoveWindow(rectENCs);

		// width control
		m_ListCurrentSelection.SetColumnWidth(0, (int)(rectENCs.Width()*0.10));						
		m_ListCurrentSelection.SetColumnWidth(1, (int)(rectENCs.Width() * 0));						
		m_ListCurrentSelection.SetColumnWidth(2, (int)(rectENCs.Width()*0.30));						
		m_ListCurrentSelection.SetColumnWidth(3, (int)(rectENCs.Width()*0.10));						
		m_ListCurrentSelection.SetColumnWidth(4, (int)(rectENCs.Width()*0.15));						
		m_ListCurrentSelection.SetColumnWidth(5, (int)(rectENCs.Width()*0.15));						
		m_ListCurrentSelection.SetColumnWidth(6, (int)(rectENCs.Width()*0.10));						
		m_ListCurrentSelection.SetColumnWidth(7, (int)(rectENCs.Width()*0.10));
	}
}

void CDialogDockCurrentSelection::OnMenuRemoveAll()
{
	m_ListCurrentSelection.DeleteAllItems();
}
