
#include "pch.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "DATASET_SUPPORTdlg.h"
#include "../LibMFCUtil/LibMFCUtil.h"
#include "DataSetManagerSupport.h"



IMPLEMENT_DYNAMIC(DATASET_SUPPORTdlg, CDialogEx)

DATASET_SUPPORTdlg::DATASET_SUPPORTdlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DATASET_SUPPORT, pParent)
{

}

DATASET_SUPPORTdlg::~DATASET_SUPPORTdlg()
{
}

void DATASET_SUPPORTdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
}


BEGIN_MESSAGE_MAP(DATASET_SUPPORTdlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &DATASET_SUPPORTdlg::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &DATASET_SUPPORTdlg::OnNMRClickList2)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD, &DATASET_SUPPORTdlg::OnMenuLoad)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE32778, &DATASET_SUPPORTdlg::OnMenuDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENFOLDER, &DATASET_SUPPORTdlg::OnMenuOpenFolder)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &DATASET_SUPPORTdlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &DATASET_SUPPORTdlg::OnNMCustomdrawList2)
END_MESSAGE_MAP()




BOOL DATASET_SUPPORTdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;

	m_list1.GetClientRect(rect);
	m_list1.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 135);
	m_list1.InsertColumn(1, _T("FilePath"), LVCFMT_LEFT, 275);
	

	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	m_list2.GetClientRect(rect);
	m_list2.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 135);
	m_list2.InsertColumn(1, _T("FilePath"), LVCFMT_LEFT, 275);

	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);



	// Get Files
	auto DSItems = DataSetManagerSupport::GetInstance().selectAllDataDS_SP("DatasetFiles");
	for each (auto var in DSItems)
	{
		if (var)
		{
			m_vecDS.push_back(var);
			Set_Item(var);
		}
	}

	auto SPItems = DataSetManagerSupport::GetInstance().selectAllDataDS_SP("SupportFiles");
	for each (auto var in SPItems)
	{
		if (var)
		{
			m_vecSP.push_back(var);
			Set_Item(var);
		}
	}

	return TRUE;
}


void DATASET_SUPPORTdlg::OnMenuLoad(CCmdUI* pCmdUI)
{
	CFileDialog dlg(TRUE,
		_T("xml"),
		NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("xml (*.xml)|*.xml|"));

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		LoadFile(filePath);
	}
}

void DATASET_SUPPORTdlg::OnMenuDelete(CCmdUI* pCmdUI)
{
	CWnd* pFocus = GetFocus();

	if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list1.GetDlgCtrlID())
	{
		DeleteSelectedItems(m_list1, m_vecDS);
	}
	else if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list2.GetDlgCtrlID())
	{
		DeleteSelectedItems(m_list2, m_vecSP);
	}
}


void DATASET_SUPPORTdlg::OnMenuOpenFolder(CCmdUI * pCmdUI)
{
	CWnd* pFocus = GetFocus();

	if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list1.GetDlgCtrlID())
	{
		OpenFolder(m_list1, m_vecDS);
	}
	else if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list2.GetDlgCtrlID())
	{
		OpenFolder(m_list2, m_vecSP);
	}
}

void DATASET_SUPPORTdlg::OpenFolder(CListCtrl& listControl, std::vector<std::shared_ptr<DatasetClass>>& vec)
{

	int nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
	while (nItem != -1)
	{
		if (nItem >= 0 && nItem < vec.size()) {
			DataSetManagerSupport::GetInstance().OpenFolder(LibMFCUtil::StringToWString(vec[nItem]->filePath).c_str());
		}

		nItem = listControl.GetNextItem(nItem, LVNI_SELECTED);
	}


	//int nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
	//while (nItem != -1)
	//{
	//	if (nItem >= 0 && nItem < vec.size()) {
	//		DataSetManagerSupport::GetInstance().OpenFolder(LibMFCUtil::StringToWString(vec[nItem]->filePath).c_str());
	//	}

	//	nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
	//}
}

void DATASET_SUPPORTdlg::DeleteSelectedItems(CListCtrl& listControl, std::vector<std::shared_ptr<DatasetClass>>& vec)
{
	int nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
	while (nItem != -1)
	{
		listControl.DeleteItem(nItem);

		if (nItem >= 0 && nItem < vec.size()) {
			vec.erase(vec.begin() + nItem);
		}

		nItem = listControl.GetNextItem(-1, LVNI_SELECTED);
	}
}


void DATASET_SUPPORTdlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_DATASET_MANAGER);
	CMenu* pPopupMenu = menu.GetSubMenu(0);

	if (pPopupMenu != nullptr)
	{
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}


void DATASET_SUPPORTdlg::OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_DATASET_MANAGER);
	CMenu* pPopupMenu = menu.GetSubMenu(0);

	if (pPopupMenu != nullptr)
	{
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}

void DATASET_SUPPORTdlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

	for (UINT i = 0; i < nFiles; i++)
	{
		UINT nPathLength = ::DragQueryFile(hDropInfo, i, NULL, 0);
		CString filePath;
		::DragQueryFile(hDropInfo, i, filePath.GetBufferSetLength(nPathLength), nPathLength + 1);
		filePath.ReleaseBuffer();

		LoadFile(filePath);
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

void DATASET_SUPPORTdlg::OnDestroy()
{
	DataSetManagerSupport::GetInstance().deleteAllDataFromTable("DatasetFiles");
	DataSetManagerSupport::GetInstance().deleteAllDataFromTable("SupportFiles");

	for (int i = 0; i < m_vecDS.size(); i++)
	{
		DataSetManagerSupport::GetInstance().insertData("DatasetFiles", m_vecDS[i]->fileName, m_vecDS[i]->filePath);

		auto layers = theApp.gisLib->GetLayerManager()->layers;
		bool isEnable = false;
		for (auto iter = layers.begin(); iter != layers.end(); iter++)
		{
			auto layer = *iter;
			if (layer->GetLayerPath().Compare(LibMFCUtil::StringToWString(m_vecDS[i]->filePath).c_str()) == 0)
				isEnable = true;
		}
		if (!isEnable)
			theApp.gisLib->GetLayerManager()->AddLayer(LibMFCUtil::StringToWString(m_vecDS[i]->filePath).c_str());
	}
	theApp.m_pDockablePaneLayerManager.UpdateList();

	for (int i = 0; i < m_vecSP.size(); i++)
		DataSetManagerSupport::GetInstance().insertData("SupportFiles", m_vecSP[i]->fileName, m_vecSP[i]->filePath);



	CDialogEx::OnDestroy();
}


void DATASET_SUPPORTdlg::SetRefreshItem()
{
	m_list1.DeleteAllItems();
	for (auto item : m_vecDS)
		Set_Item(item);

	m_list2.DeleteAllItems();
	for (auto item : m_vecSP)
		Set_Item(item);
}

void DATASET_SUPPORTdlg::Set_Item(std::shared_ptr<DatasetClass> item)
{
	if (item)
	{
		if (item->DS_SP)
		{
			int size = m_list2.GetItemCount();
			m_list2.InsertItem(size, _T(""));
			m_list2.SetItemText(size, 0, LibMFCUtil::StringToWString(item->fileName).c_str());
			m_list2.SetItemText(size, 1, LibMFCUtil::StringToWString(item->filePath).c_str());

			m_list2.SetCheck(size, false);
		}
		else
		{
			int size = m_list1.GetItemCount();
			m_list1.InsertItem(size, _T(""));
			m_list1.SetItemText(size, 0, LibMFCUtil::StringToWString(item->fileName).c_str());
			m_list1.SetItemText(size, 1, LibMFCUtil::StringToWString(item->filePath).c_str());

			m_list1.SetCheck(size, false);
		}
	}
}


void DATASET_SUPPORTdlg::LoadFile(CString filePath)
{
	if ((filePath.Find(L".xml") != std::wstring::npos) || (filePath.Find(L"XML") != std::wstring::npos))
	{
		CString fileName;
		int pos = filePath.ReverseFind('\\');
		if (pos != -1) {
			fileName = filePath.Mid(pos + 1);
		}

		auto temp = DataSetManagerSupport::GetInstance().Dataset_LoadFile(LibMFCUtil::WStringToString(filePath.GetBuffer()));
		if (temp.size())
		{
			for (int i = 0; i < temp.size(); i++)
			{
				if (temp[i])
				{
					if (temp[i]->DS_SP)
					{
						m_vecSP.push_back(temp[i]);
					}
					else
					{
						m_vecDS.push_back(temp[i]);
					}
				}
			}
		}
		SetRefreshItem();
	}
}


BOOL DATASET_SUPPORTdlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void DATASET_SUPPORTdlg::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CString strFilePath = m_list1.GetItemText(lplvcd->nmcd.dwItemSpec, 1); 

		if (!PathFileExists(strFilePath))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}

		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}


void DATASET_SUPPORTdlg::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CString strFilePath = m_list2.GetItemText(lplvcd->nmcd.dwItemSpec, 1); 

		if (!PathFileExists(strFilePath))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}

		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}
