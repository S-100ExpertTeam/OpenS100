#include "pch.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "FC_PCSelectdlg.h"
#include "string.h"



#include "../LibMFCUtil/LibMFCUtil.h"


#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

IMPLEMENT_DYNAMIC(FC_PCSelectdlg, CDialogEx)

FC_PCSelectdlg::FC_PCSelectdlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FC_PC, pParent)
{

}

FC_PCSelectdlg::~FC_PCSelectdlg()
{
}

void FC_PCSelectdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST2, m_list2);
}

BEGIN_MESSAGE_MAP(FC_PCSelectdlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &FC_PCSelectdlg::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &FC_PCSelectdlg::OnNMRClickList2)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD, &FC_PCSelectdlg::OnMenuLoad)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE32778, &FC_PCSelectdlg::OnMenuDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENFOLDER, &FC_PCSelectdlg::OnMenuOpenFolder)
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &FC_PCSelectdlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &FC_PCSelectdlg::OnNMCustomdrawList2)
END_MESSAGE_MAP()


BOOL FC_PCSelectdlg::OnInitDialog()
{
	//init 
	CDialogEx::OnInitDialog();
	CRect rect;

	m_list1.GetClientRect(rect);
	m_list1.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 245);
	m_list1.InsertColumn(1, _T("Product"), LVCFMT_LEFT, 100);
	m_list1.InsertColumn(2, _T("Version"), LVCFMT_LEFT, 75);
	m_list1.InsertColumn(3, _T("Filepath"), LVCFMT_LEFT, 0);

	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	m_list2.GetClientRect(rect);
	m_list2.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 245);
	m_list2.InsertColumn(1, _T("Product"), LVCFMT_LEFT, 100);
	m_list2.InsertColumn(2, _T("Version"), LVCFMT_LEFT, 75);
	m_list2.InsertColumn(3, _T("Filepath"), LVCFMT_LEFT, 0);

	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	// Get Files
	auto FCItems= DataSetManagerSupport::GetInstance().selectAllData("FeatureCatalogue");
	for each (auto var in FCItems)
	{
		if (var)
		{
			m_vecFC.push_back(var);
			SetFC_PC_Item(var);
		}
	}

	auto PCItems = DataSetManagerSupport::GetInstance().selectAllData("PortrayalCatalogue");
	for each (auto var in PCItems)
	{
		if (var)
		{
			m_vecPC.push_back(var);
			SetFC_PC_Item(var);
		}
	}

	return TRUE; 
}


void FC_PCSelectdlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_DATASET_MANAGER);
	CMenu* pPopupMenu = menu.GetSubMenu(0);

	if (pPopupMenu != nullptr)
	{
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN  | TPM_RIGHTBUTTON , point.x, point.y, this);
	}

	*pResult = 0;
}


void FC_PCSelectdlg::OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult)
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


void FC_PCSelectdlg::OnMenuLoad(CCmdUI* pCmdUI)
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

void FC_PCSelectdlg::OnMenuDelete(CCmdUI* pCmdUI)
{

	CWnd* pFocus = GetFocus();

	if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list1.GetDlgCtrlID())
	{
		DeleteSelectedItems(m_list1, m_vecFC);
	}
	else if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list2.GetDlgCtrlID())
	{
		DeleteSelectedItems(m_list2, m_vecPC);
	}
}

void FC_PCSelectdlg::DeleteSelectedItems(CListCtrl& listControl, std::vector<std::shared_ptr<FC_PC_DatasetClass>>& vec)
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

void FC_PCSelectdlg::OnClose()
{
	//close
	//CDialogEx::OnClose();
}


std::vector<int> FC_PCSelectdlg::parseVersion(const std::string& version) {
	std::vector<int> parts;
	std::stringstream ss(version);
	std::string part;
	while (std::getline(ss, part, '.')) {
		try {
			parts.push_back(std::stoi(part));
		}
		catch (const std::invalid_argument& e) {
			return {};
		}
	}
	return parts;
}

bool FC_PCSelectdlg::isValidVersion(const std::string& version) {
	return !parseVersion(version).empty();
}

int FC_PCSelectdlg::compareVersions(const std::string& v1, const std::string& v2) {
	std::vector<int> v1Parts = parseVersion(v1);
	std::vector<int> v2Parts = parseVersion(v2);

	for (size_t i = 0; i < (((v1Parts.size()) >(v2Parts.size())) ? (v1Parts.size()) : (v2Parts.size())); ++i) {
		int v1Part = i < v1Parts.size() ? v1Parts[i] : 0;
		int v2Part = i < v2Parts.size() ? v2Parts[i] : 0;
		if (v1Part < v2Part) return -1;
		else if (v1Part > v2Part) return 1;
	}
	return 0;
}

std::string FC_PCSelectdlg::chooseVersion(const std::string& current, const std::string& newVersion) {
	if (!isValidVersion(current) || !isValidVersion(newVersion) || compareVersions(current, newVersion) != 0) {
		return current;
	}
	return newVersion;
}


void FC_PCSelectdlg::SetRefreshItem()
{
	m_list1.DeleteAllItems();
	for(auto item :m_vecFC)
		SetFC_PC_Item(item);

	m_list2.DeleteAllItems();
	for (auto item : m_vecPC)
		SetFC_PC_Item(item);
}

void FC_PCSelectdlg::OnMenuOpenFolder(CCmdUI* pCmdUI)
{
	CWnd* pFocus = GetFocus();

	if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list1.GetDlgCtrlID())
	{
		OpenFolder(m_list1, m_vecFC);
	}
	else if (pFocus != nullptr && pFocus->GetDlgCtrlID() == m_list2.GetDlgCtrlID())
	{
		OpenFolder(m_list2, m_vecPC);
	}
}

void FC_PCSelectdlg::OpenFolder(CListCtrl& listControl, std::vector<std::shared_ptr<FC_PC_DatasetClass>>& vec)
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


void FC_PCSelectdlg::SetFC_PC_Item(std::shared_ptr<FC_PC_DatasetClass> item)
{
	if (item)
	{
		if (item->FC_PC)
		{
			int size = m_list2.GetItemCount();
			m_list2.InsertItem(size, _T(""));
			m_list2.SetItemText(size, 0, LibMFCUtil::StringToWString(item->fileName).c_str());
			m_list2.SetItemText(size, 1, LibMFCUtil::StringToWString(item->product).c_str());
			m_list2.SetItemText(size, 2, LibMFCUtil::StringToWString(item->version).c_str());
			m_list2.SetItemText(size, 3, LibMFCUtil::StringToWString(item->filePath).c_str());

			m_list2.SetCheck(size, false);
		}
		else
		{
			int size = m_list1.GetItemCount();
			m_list1.InsertItem(size, _T(""));
			m_list1.SetItemText(size, 0, LibMFCUtil::StringToWString(item->fileName).c_str());
			m_list1.SetItemText(size, 1, LibMFCUtil::StringToWString(item->product).c_str());
			m_list1.SetItemText(size, 2, LibMFCUtil::StringToWString(item->version).c_str());
			m_list1.SetItemText(size, 3, LibMFCUtil::StringToWString(item->filePath).c_str());

			m_list1.SetCheck(size, false);
		}
	}
}


void FC_PCSelectdlg::LoadFile(CString filePath)
{
	if ((filePath.Find(L".xml") != std::wstring::npos) || (filePath.Find(L"XML") != std::wstring::npos))
	{
		CString fileName;
		int pos = filePath.ReverseFind('\\');
		if (pos != -1) {
			fileName = filePath.Mid(pos + 1);
		}

		auto temp = DataSetManagerSupport::GetInstance().FC_PC_LoadFile(LibMFCUtil::WStringToString(filePath.GetBuffer()));
		if (temp)
		{
			temp->fileName = LibMFCUtil::WStringToString(fileName.GetBuffer());
			temp->filePath = LibMFCUtil::WStringToString(filePath.GetBuffer());
			
			if (temp->FC_PC)
			{
				auto data = DataSetManagerSupport::GetInstance().selectDataByFilePath("PortrayalCatalogue", temp->filePath);
				if (data)
				{
					int res = compareVersions(data->version, temp->version);
					if (res == 0) {
						return;
					}
					else
					{
						if (res != 1)
							return;
						else
							m_vecPC.erase(std::remove(m_vecPC.begin(), m_vecPC.end(), data), m_vecPC.end());
					}
				}
				m_vecPC.push_back(temp);
			}
			else
			{
				auto data = DataSetManagerSupport::GetInstance().selectDataByFilePath("FeatureCatalogue", temp->filePath);
				if (data)
				{
					int res = compareVersions(data->version, temp->version);
					if (res == 0) {
						return;
					}
					else
					{
						if (res != 1)
							return;
						else
							m_vecFC.erase(std::remove(m_vecFC.begin(), m_vecFC.end(), data), m_vecFC.end());
					}
				}
				m_vecFC.push_back(temp);
			}
			SetRefreshItem();
		}
	}
}


void FC_PCSelectdlg::OnDropFiles(HDROP hDropInfo)
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


BOOL FC_PCSelectdlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void FC_PCSelectdlg::OnDestroy() 
{
 	auto FcList = ct->getFcProductList();
	auto PcList = ct->getPcProductList();


	DataSetManagerSupport::GetInstance().deleteAllDataFromTable("FeatureCatalogue");
	DataSetManagerSupport::GetInstance().deleteAllDataFromTable("PortrayalCatalogue");

	for (int i = 0; i < m_vecFC.size(); i++)
	{
		bool enable = false;
		for (auto fc : FcList)
		{
			if (fc == "")
			{
			}
			else if(std::string::npos != m_vecFC[i]->product.find(fc))
			{
				enable = true;
				break;
			}
		}

		if (!enable)
			ct->addFC(m_vecFC[i]->filePath);

		DataSetManagerSupport::GetInstance().insertData("FeatureCatalogue", m_vecFC[i]->fileName, m_vecFC[i]->filePath
			, m_vecFC[i]->product, m_vecFC[i]->version);
	}


	for (int i = 0; i < m_vecPC.size(); i++)
	{
		bool enable = false;
		for (auto pc : PcList)
		{
			if (pc == "")
			{
			}
			else if (std::string::npos != m_vecPC[i]->product.find(pc))
			{
				enable = true;
				break;
			}
		}

		if (!enable)
			ct->addPC(m_vecPC[i]->filePath);

		DataSetManagerSupport::GetInstance().insertData("PortrayalCatalogue", m_vecPC[i]->fileName, m_vecPC[i]->filePath
			, m_vecPC[i]->product, m_vecPC[i]->version);
	}

	CDialogEx::OnDestroy();
}


void FC_PCSelectdlg::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CString strFilePath = m_list1.GetItemText(lplvcd->nmcd.dwItemSpec, 3); 

		if (!PathFileExists(strFilePath))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}

		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}


void FC_PCSelectdlg::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CString strFilePath = m_list2.GetItemText(lplvcd->nmcd.dwItemSpec, 3);

		if (!PathFileExists(strFilePath))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}

		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}
