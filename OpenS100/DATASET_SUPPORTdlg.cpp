#include "pch.h"
#include "../GISLibrary/SystemDatabaseSession.h"
#include "../SystemDatabase/CatalogueResolution.h"
#include "../SystemDatabase/Utf8Path.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "DATASET_SUPPORTdlg.h"
#include "DatasetManagerUi.h"

#include <format>

IMPLEMENT_DYNAMIC(DATASET_SUPPORTdlg, CDialogEx)

namespace
{
	enum DatasetColumn { DatasetName, DatasetEdition, DatasetIssued, DatasetStatus, DatasetPath };
	enum SupportColumn { SupportName, SupportPath };

	bool isExchangeCatalogue(const std::filesystem::path& file)
	{
		std::wstring name = file.filename().wstring();
		for (auto& c : name)
		{
			c = static_cast<wchar_t>(::towupper(c));
		}
		return name == L"CATALOG.XML";
	}

	// The product a support file belongs to, from its name: S-101 support files are named
	// "101<producer>...". Files that do not say go with S-100 material in general.
	std::string supportFileProduct(const std::filesystem::path& file)
	{
		const std::string name = s100::sdb::toUtf8(file.filename());
		const std::string product = name.size() >= 3 ? s100::sdb::productFromIdentifier(name.substr(0, 3)) : std::string();
		return product.empty() ? "S-100" : product;
	}
}

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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &DATASET_SUPPORTdlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &DATASET_SUPPORTdlg::OnNMCustomdrawList2)
END_MESSAGE_MAP()


BOOL DATASET_SUPPORTdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list1.InsertColumn(DatasetName, _T("Name"), LVCFMT_LEFT, 135);
	m_list1.InsertColumn(DatasetEdition, _T("Edition"), LVCFMT_LEFT, 60);
	m_list1.InsertColumn(DatasetIssued, _T("Issued"), LVCFMT_LEFT, 85);
	m_list1.InsertColumn(DatasetStatus, _T("Status"), LVCFMT_LEFT, 70);
	m_list1.InsertColumn(DatasetPath, _T("FilePath"), LVCFMT_LEFT, 275);
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_list2.InsertColumn(SupportName, _T("Name"), LVCFMT_LEFT, 135);
	m_list2.InsertColumn(SupportPath, _T("FilePath"), LVCFMT_LEFT, 275);
	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	SetRefreshItem();
	return TRUE;
}

void DATASET_SUPPORTdlg::SetRefreshItem()
{
	const auto* session = theApp.systemDatabase.get();
	if (!session)
	{
		return;
	}
	auto& database = theApp.systemDatabase->database();
	m_vecDS = database.datasets().all();
	m_vecSP = database.supportFiles().all();

	m_list1.DeleteAllItems();
	for (int row = 0; row < static_cast<int>(m_vecDS.size()); ++row)
	{
		const auto& dataset = m_vecDS[static_cast<std::size_t>(row)];
		std::string edition = dataset.edition ? std::to_string(*dataset.edition) : std::string("-");
		edition += "." + std::to_string(dataset.updateNumber);
		for (const auto& update : database.datasets().updates(dataset.id))
		{
			edition += std::format(" +{}", update.updateNumber);
		}
		m_list1.InsertItem(row, DatasetManagerUi::Widen(dataset.name).c_str());
		m_list1.SetItemText(row, DatasetEdition, DatasetManagerUi::Widen(edition).c_str());
		m_list1.SetItemText(row, DatasetIssued, DatasetManagerUi::Widen(dataset.issueDate).c_str());
		m_list1.SetItemText(row, DatasetStatus, DatasetManagerUi::Widen(std::string(s100::sdb::toString(dataset.status))).c_str());
		m_list1.SetItemText(row, DatasetPath, session->store().absolute(dataset.path).c_str());
	}

	m_list2.DeleteAllItems();
	for (int row = 0; row < static_cast<int>(m_vecSP.size()); ++row)
	{
		const auto& supportFile = m_vecSP[static_cast<std::size_t>(row)];
		m_list2.InsertItem(row, DatasetManagerUi::Widen(supportFile.name).c_str());
		m_list2.SetItemText(row, SupportPath, session->store().absolute(supportFile.path).c_str());
	}
}

void DATASET_SUPPORTdlg::ShowContextMenu()
{
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_DATASET_MANAGER);
	CMenu* pPopupMenu = menu.GetSubMenu(0);
	if (pPopupMenu != nullptr)
	{
		pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void DATASET_SUPPORTdlg::OnNMRClickList1(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ShowContextMenu();
	*pResult = 0;
}

void DATASET_SUPPORTdlg::OnNMRClickList2(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ShowContextMenu();
	*pResult = 0;
}

void DATASET_SUPPORTdlg::OnMenuLoad(CCmdUI* /*pCmdUI*/)
{
	CFileDialog dlg(TRUE,
		nullptr,
		NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		_T("Exchange set catalogue (CATALOG.XML)|CATALOG.XML|Datasets and updates (*.000;*.001;...)|*.0*;*.1*;*.2*;*.3*;*.4*;*.5*;*.6*;*.7*;*.8*;*.9*|All files (*.*)|*.*|"));

	if (dlg.DoModal() == IDOK)
	{
		std::vector<std::filesystem::path> files;
		for (POSITION position = dlg.GetStartPosition(); position != nullptr;)
		{
			files.emplace_back(dlg.GetNextPathName(position).GetString());
		}
		LoadFiles(files);
	}
}

void DATASET_SUPPORTdlg::LoadFiles(const std::vector<std::filesystem::path>& files)
{
	auto* session = DatasetManagerUi::Session(this);
	if (!session)
	{
		return;
	}

	CWaitCursor wait;
	const GISLibrary::S101DatasetInspector datasetInspector;
	std::vector<s100::sdb::ImportResult> results;
	std::vector<std::string> problems;
	for (const auto& file : files)
	{
		if (isExchangeCatalogue(file))
		{
			auto report = session->importer().importExchangeSet(file);
			results.insert(results.end(), report.results.begin(), report.results.end());
			problems.insert(problems.end(), report.installProblems.begin(), report.installProblems.end());
		}
		else if (datasetInspector.accepts(file))
		{
			results.push_back(session->importer().importDataset(file));
		}
		else
		{
			results.push_back(session->importer().importSupportFile(file, supportFileProduct(file), "new"));
		}
	}
	session->refreshDatasets();
	SetRefreshItem();
	theApp.MapRefresh();
	theApp.m_pDockablePaneLayerManager.UpdateList();
	DatasetManagerUi::Report(this, results, problems);
}

std::vector<std::size_t> DATASET_SUPPORTdlg::SelectedRows(bool& datasets)
{
	CWnd* focus = GetFocus();
	datasets = !(focus && focus->GetDlgCtrlID() == m_list2.GetDlgCtrlID());
	CListCtrl& list = datasets ? m_list1 : m_list2;
	const std::size_t count = datasets ? m_vecDS.size() : m_vecSP.size();

	std::vector<std::size_t> rows;
	for (int item = list.GetNextItem(-1, LVNI_SELECTED); item != -1; item = list.GetNextItem(item, LVNI_SELECTED))
	{
		if (static_cast<std::size_t>(item) < count)
		{
			rows.push_back(static_cast<std::size_t>(item));
		}
	}
	return rows;
}

void DATASET_SUPPORTdlg::OnMenuDelete(CCmdUI* /*pCmdUI*/)
{
	auto* session = DatasetManagerUi::Session(this);
	bool datasets = true;
	const auto rows = SelectedRows(datasets);
	if (!session || rows.empty() || !DatasetManagerUi::ConfirmRemoval(this, rows.size()))
	{
		return;
	}
	CWaitCursor wait;
	for (const std::size_t row : rows)
	{
		if (datasets)
		{
			(void)session->importer().removeDataset(m_vecDS[row].id);
		}
		else
		{
			(void)session->importer().removeSupportFile(m_vecSP[row].id);
		}
	}
	session->refreshDatasets();
	SetRefreshItem();
	theApp.MapRefresh();
}

void DATASET_SUPPORTdlg::OnMenuOpenFolder(CCmdUI* /*pCmdUI*/)
{
	const auto* session = theApp.systemDatabase.get();
	if (!session)
	{
		return;
	}
	bool datasets = true;
	for (const std::size_t row : SelectedRows(datasets))
	{
		DatasetManagerUi::ShowInFolder(session->store().absolute(datasets ? m_vecDS[row].path : m_vecSP[row].path));
	}
}

void DATASET_SUPPORTdlg::OnDropFiles(HDROP hDropInfo)
{
	LoadFiles(DatasetManagerUi::DroppedFiles(hDropInfo));
	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL DATASET_SUPPORTdlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void DATASET_SUPPORTdlg::HighlightMissing(CListCtrl& list, int pathColumn, NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		const CString path = list.GetItemText(static_cast<int>(lplvcd->nmcd.dwItemSpec), pathColumn);
		if (!PathFileExists(path))
		{
			lplvcd->clrTextBk = RGB(223, 159, 160);
		}
		*pResult = CDRF_DODEFAULT;
	}
	break;
	}
}

void DATASET_SUPPORTdlg::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	HighlightMissing(m_list1, DatasetPath, pNMHDR, pResult);
}

void DATASET_SUPPORTdlg::OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	HighlightMissing(m_list2, SupportPath, pNMHDR, pResult);
}
