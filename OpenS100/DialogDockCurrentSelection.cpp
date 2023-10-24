#include "pch.h"
#include "DialogDockCurrentSelection.h"
#include "DialogDockRelation.h"
#include "OpenS100.h"
#include "OpenS100View.h"

#include <vector>

#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/GISLibrary.h"
#include "../GISLibrary/SPoint.h"

#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../FeatureCatalog/FeatureType.h"

#define COL_INDEX_ID 0 
#define COL_INDEX_NAME 1
#define COL_INDEX_GEOMETRY 2
#define COL_INDEX_LAT 3
#define COL_INDEX_LON 4
#define COL_INDEX_RELATION_CNT 5
#define COL_INDEX_TYPE 6

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

// Current Selection item has been selected.
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

			nSelectedItem = pNMLV->iItem;

			if (m_Cell)
			{
				auto cell = m_Cell;
				FeatureCatalogue* fc = ((S100Layer*)cell->m_pLayer)->GetFeatureCatalog();
				if (nullptr == fc || nullptr == cell)
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
				objId.Format(m_ListCurrentSelection.GetItemText(idx, COL_INDEX_ID));

				// Feature or Information
				featureType.Format(m_ListCurrentSelection.GetItemText(idx, COL_INDEX_TYPE));
				//__int64 objIdN = _tcstoui64(objId, NULL, 10);

				if (featureType == L"Feature")
				{
					//__int64 key = ((__int64)100) << 32 | objIdN;

					//auto stringKey = std::to_wstring(key);

					CString geoType = m_ListCurrentSelection.GetItemText(idx, COL_INDEX_GEOMETRY);

					if (geoType != L"No geometry")
					{
						auto layer = cell->GetLayer();
						if (nullptr != layer)
						{
							if (true == layer->IsOn())
							{
								//theApp.pView->SetPick(cell, stringKey);
								theApp.pView->SetPick(cell, std::wstring(objId));
								theApp.pView->Invalidate(FALSE);
							}
						}
					}

					theApp.m_DockablePaneEditWindow.SetSpatialObject(cell);
					//theApp.m_DockablePaneEditWindow.SetFeatureType(stringKey);
					theApp.m_DockablePaneEditWindow.SetFeatureType(std::wstring(objId));
				}
				else if (featureType == L"Information")
				{
					//__int64 key = ((__int64)150) << 32 | objIdN;
					//auto stringKey = std::to_wstring(key);

					theApp.m_DockablePaneEditWindow.SetSpatialObject(cell);
					//theApp.m_DockablePaneEditWindow.SetInformationType(stringKey);
					theApp.m_DockablePaneEditWindow.SetInformationType(std::wstring(objId));
				}

				pos = m_ListCurrentSelection.GetFirstSelectedItemPosition();
				idx = 0;

				std::list<GF::FeatureType*> flist;
				std::list<GF::InformationType*> infoList;

				while (pos)
				{
					CString objId;
					CString featureType;
					idx = m_ListCurrentSelection.GetNextSelectedItem(pos);
					objId.Format(m_ListCurrentSelection.GetItemText(idx, COL_INDEX_ID));
					featureType.Format(m_ListCurrentSelection.GetItemText(idx, COL_INDEX_TYPE)); // feature/information type
					//__int64 objIdN = _tcstoui64(objId, NULL, 10);

					if (featureType == L"Feature")
					{
						//__int64 key = ((__int64)100) << 32 | objIdN;
						//auto feature = cell->GetFeatureType(std::to_string(key));
						auto feature = cell->GetFeatureType(pugi::as_utf8(std::wstring(objId)));
						if (feature)
						{
							flist.push_back(feature);
						}
					}
					else if (featureType == L"Information")
					{
						//__int64 key = ((__int64)150) << 32 | objIdN;
						//auto information = cell->GetInformationType(std::to_string(key));
						auto information = cell->GetInformationType(pugi::as_utf8(std::wstring(objId)));
						if (information)
						{
							infoList.push_back(information);
						}
					}
				}

				theApp.m_DockablePaneRelation.pDlg->SetFeatureList(cell, flist, infoList);
			}
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

// current selection
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
	m_ListCurrentSelection.InsertColumn(COL_INDEX_ID, _T("ID "), LVCFMT_LEFT, (int)(listRect.Width() * 0.10));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_NAME, _T("Name"), LVCFMT_LEFT, (int)(listRect.Width() * 0.30));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_GEOMETRY, _T("Geometry"), LVCFMT_LEFT, (int)(listRect.Width() * 0.10));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_LAT, _T("Lat"), LVCFMT_CENTER, (int)(listRect.Width() * 0.15));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_LON, _T("Lon"), LVCFMT_CENTER, (int)(listRect.Width() * 0.15));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_RELATION_CNT, _T("Relation Cnt"), LVCFMT_LEFT, (int)(listRect.Width() * 0.10));
	m_ListCurrentSelection.InsertColumn(COL_INDEX_TYPE, _T("Type"), LVCFMT_LEFT, (int)(listRect.Width() * 0.10));

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogDockCurrentSelection::UpdateListTest(CStringArray *csa, S100SpatialObject* cell, CString isCtrlClicked)
{
	CString tmp;
	m_Cell = cell;

	// If ctrl is not pressed, delete the item and start;
	if (isCtrlClicked == L"0")
	{
		m_ListCurrentSelection.DeleteAllItems();
	}
	
	auto fc = cell->GetFC();
	
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

			if (csType == "1")
			{
				type = "Point";
			}
			else if (csType == "2" || csType == "5" || csType == "7")
			{
				type = "Curve";
			}
			else if (csType == "3") 
			{
				type = "Surface";
			}
			else if (csType == "4")
			{
				type = "Multi Point";
			}
			else if (csType == "999") 
			{
				type = "No Geometry";
			}
			else 
			{
				type = "-";
			}

			bool isExist = false;

			// duplicate check (frid)
			for (int j = 0; j < m_ListCurrentSelection.GetItemCount(); j++)
			{
				if (frid == m_ListCurrentSelection.GetItemText(j, COL_INDEX_ID) && featureType == m_ListCurrentSelection.GetItemText(j, COL_INDEX_TYPE))
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
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_ID, frid);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_NAME, name);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_GEOMETRY, type);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_LAT, lat);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_LON, lon);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_RELATION_CNT, assoCnt);
				m_ListCurrentSelection.SetItemText(0, COL_INDEX_TYPE, featureType);

				if (featureType == L"Feature")
				{
					// Acquired a catalog.
					auto it = fc->GetFeatureTypes()->GetFeatureType().find(ws_name);
					if (it == fc->GetFeatureTypes()->GetFeatureType().end())
					{
						CString msg;
						msg.Format(L"[%s] Feature not found. -CURRENT SELECTION", name.GetBuffer());
					}
					else
					{
						FeatureType* ft = it->second;
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

long long CDialogDockCurrentSelection::GetSelectedRecordName()
{
	if (nSelectedItem >= 0 && nSelectedItem < m_ListCurrentSelection.GetItemCount())
	{
		auto strRCID = m_ListCurrentSelection.GetItemText(nSelectedItem, COL_INDEX_ID);
		auto strType = m_ListCurrentSelection.GetItemText(nSelectedItem, COL_INDEX_TYPE);

		auto rcid = _ttoi(strRCID);
		int rcnm = 0;
		if (strType.Compare(L"Feature") == 0)
		{
			rcnm = 100;
			RecordName recordName(rcnm, rcid);
			return recordName.GetName();
		}
		else if (strType.Compare(L"Information") == 0)
		{
			rcnm = 150;
			RecordName recordName(rcnm, rcid);
			return recordName.GetName();
		}
	}

	return -1;
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
		auto itemid = m_ListCurrentSelection.GetItemText(i, COL_INDEX_ID);
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
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_ID, (int)(rectENCs.Width() * 0.10));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_NAME, (int)(rectENCs.Width() * 0.30));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_GEOMETRY, (int)(rectENCs.Width() * 0.10));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_LAT, (int)(rectENCs.Width() * 0.10));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_LON, (int)(rectENCs.Width() * 0.10));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_RELATION_CNT, (int)(rectENCs.Width() * 0.10));
		m_ListCurrentSelection.SetColumnWidth(COL_INDEX_TYPE, (int)(rectENCs.Width() * 0.20));
	}
}

void CDialogDockCurrentSelection::OnMenuRemoveAll()
{
	m_ListCurrentSelection.DeleteAllItems();
}
