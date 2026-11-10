#include "pch.h"
#include "../GISLibrary/SystemDatabaseSession.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "FC_PCSelectdlg.h"
#include "DatasetManagerUi.h"

IMPLEMENT_DYNAMIC(FC_PCSelectdlg, CDialogEx)

namespace
{
	enum Column { NameColumn, ProductColumn, VersionColumn, PathColumn };
}

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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &FC_PCSelectdlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &FC_PCSelectdlg::OnNMCustomdrawList2)
END_MESSAGE_MAP()


BOOL FC_PCSelectdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (CListCtrl* list : { &m_list1, &m_list2 })
	{
		list->InsertColumn(NameColumn, _T("Name"), LVCFMT_LEFT, 245);
		list->InsertColumn(ProductColumn, _T("Product"), LVCFMT_LEFT, 100);
		list->InsertColumn(VersionColumn, _T("Version"), LVCFMT_LEFT, 75);
		list->InsertColumn(PathColumn, _T("Filepath"), LVCFMT_LEFT, 0);
		list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	}

	SetRefreshItem();
	return TRUE;
}

void FC_PCSelectdlg::FillList(CListCtrl& list, const std::vector<s100::sdb::CatalogueRecord>& records)
{
	const auto* session = theApp.systemDatabase.get();
	list.DeleteAllItems();
	for (int row = 0; row < static_cast<int>(records.size()); ++row)
	{
		const auto& record = records[static_cast<std::size_t>(row)];
		const auto file = session->store().absolute(record.path);
		list.InsertItem(row, file.filename().c_str());
		list.SetItemText(row, ProductColumn, DatasetManagerUi::Widen(record.product).c_str());
		list.SetItemText(row, VersionColumn, DatasetManagerUi::Widen(record.version).c_str());
		list.SetItemText(row, PathColumn, file.c_str());
	}
}

void FC_PCSelectdlg::SetRefreshItem()
{
	auto* session = theApp.systemDatabase.get();
	if (!session)
	{
		return;
	}
	m_vecFC = session->database().featureCatalogues().all();
	m_vecPC = session->database().portrayalCatalogues().all();
	FillList(m_list1, m_vecFC);
	FillList(m_list2, m_vecPC);
}

void FC_PCSelectdlg::ShowContextMenu()
{
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_DATASET_MANAGER);
	CMenu* pPopupMenu = menu.GetSubMenu(0);
	if (pPopupMenu != nullptr)
	{
		// Folders are loaded on the Datasets / SupportFiles tab.
		pPopupMenu->DeleteMenu(ID_FILE_LOADFOLDER, MF_BYCOMMAND);
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void FC_PCSelectdlg::OnNMRClickList1(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ShowContextMenu();
	*pResult = 0;
}

void FC_PCSelectdlg::OnNMRClickList2(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ShowContextMenu();
	*pResult = 0;
}

void FC_PCSelectdlg::OnMenuLoad(CCmdUI* /*pCmdUI*/)
{
	CFileDialog dlg(TRUE,
		_T("xml"),
		NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Feature or portrayal catalogue (*.xml;*.zip)|*.xml;*.zip|"));

	if (dlg.DoModal() == IDOK)
	{
		LoadFiles({ std::filesystem::path(dlg.GetPathName().GetString()) });
	}
}

void FC_PCSelectdlg::LoadFiles(const std::vector<std::filesystem::path>& files)
{
	auto* session = DatasetManagerUi::Session(this);
	if (!session)
	{
		return;
	}

	CWaitCursor wait;
	const GISLibrary::XmlCatalogueInspector inspector;
	std::vector<s100::sdb::ImportResult> results;
	for (const auto& file : files)
	{
		std::error_code error;
		std::wstring extension = file.extension().wstring();
		for (auto& c : extension)
		{
			c = static_cast<wchar_t>(::towlower(c));
		}
		if (std::filesystem::is_directory(file, error) || extension == L".zip")
		{
			results.push_back(session->importer().importPortrayalCatalogue(file));
		}
		else if (inspector.inspectFeatureCatalogue(file))
		{
			results.push_back(session->importer().importFeatureCatalogue(file));
		}
		else if (inspector.inspectPortrayalCatalogue(file))
		{
			results.push_back(session->importer().importPortrayalCatalogue(file));
		}
		else
		{
			results.push_back({ .source = file, .outcome = s100::sdb::ImportOutcome::Rejected,
				.message = "neither a feature catalogue nor a portrayal catalogue" });
		}
	}
	session->refreshCatalogues();
	SetRefreshItem();
	theApp.MapRefresh();
	DatasetManagerUi::Report(this, results);
}

std::vector<s100::sdb::CatalogueRecord> FC_PCSelectdlg::SelectedRecords(bool& featureCatalogues)
{
	CWnd* focus = GetFocus();
	featureCatalogues = !(focus && focus->GetDlgCtrlID() == m_list2.GetDlgCtrlID());
	CListCtrl& list = featureCatalogues ? m_list1 : m_list2;
	const auto& records = featureCatalogues ? m_vecFC : m_vecPC;

	std::vector<s100::sdb::CatalogueRecord> selected;
	for (int item = list.GetNextItem(-1, LVNI_SELECTED); item != -1; item = list.GetNextItem(item, LVNI_SELECTED))
	{
		if (item < static_cast<int>(records.size()))
		{
			selected.push_back(records[static_cast<std::size_t>(item)]);
		}
	}
	return selected;
}

void FC_PCSelectdlg::OnMenuDelete(CCmdUI* /*pCmdUI*/)
{
	auto* session = DatasetManagerUi::Session(this);
	bool featureCatalogues = true;
	const auto selected = SelectedRecords(featureCatalogues);
	if (!session || selected.empty() || !DatasetManagerUi::ConfirmRemoval(this, selected.size()))
	{
		return;
	}
	for (const auto& record : selected)
	{
		if (featureCatalogues)
		{
			(void)session->importer().removeFeatureCatalogue(record.id);
		}
		else
		{
			(void)session->importer().removePortrayalCatalogue(record.id);
		}
	}
	session->refreshCatalogues();
	SetRefreshItem();
	theApp.MapRefresh();
}

void FC_PCSelectdlg::OnMenuOpenFolder(CCmdUI* /*pCmdUI*/)
{
	const auto* session = theApp.systemDatabase.get();
	if (!session)
	{
		return;
	}
	bool featureCatalogues = true;
	for (const auto& record : SelectedRecords(featureCatalogues))
	{
		DatasetManagerUi::ShowInFolder(session->store().absolute(record.path));
	}
}

void FC_PCSelectdlg::OnClose()
{
	// The tab closes with the Dataset Manager, not on its own.
}

void FC_PCSelectdlg::OnDropFiles(HDROP hDropInfo)
{
	LoadFiles(DatasetManagerUi::DroppedFiles(hDropInfo));
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

void FC_PCSelectdlg::HighlightMissing(CListCtrl& list, NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		const CString path = list.GetItemText(static_cast<int>(lplvcd->nmcd.dwItemSpec), PathColumn);
		if (!PathFileExists(path))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}
		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}

void FC_PCSelectdlg::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	HighlightMissing(m_list1, pNMHDR, pResult);
}

void FC_PCSelectdlg::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	HighlightMissing(m_list2, pNMHDR, pResult);
}
