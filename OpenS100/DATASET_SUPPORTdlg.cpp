#include "pch.h"
#include "../GISLibrary/SystemDatabaseSession.h"
#include "../SystemDatabase/CatalogueResolution.h"
#include "../SystemDatabase/Utf8Path.h"
#include "OpenS100.h"
#include "afxdialogex.h"
#include "DATASET_SUPPORTdlg.h"
#include "DatasetManagerUi.h"
#include "ProtectedDataDlg.h"

#include <algorithm>
#include <format>
#include <map>

IMPLEMENT_DYNAMIC(DATASET_SUPPORTdlg, CDialogEx)

namespace
{
	enum DatasetColumn { DatasetName, DatasetProduct, DatasetEdition, DatasetIssued, DatasetStatus, DatasetPath };
	enum SupportColumn { SupportName, SupportProduct, SupportPath };

	constexpr const char* unknownProduct = "(unknown)";

	std::string datasetProduct(const s100::sdb::DatasetRecord& dataset)
	{
		return dataset.product.empty() ? std::string(unknownProduct) : dataset.product;
	}

	// Support files are stored as "SUPPORT/<product>/<file name>" (ResourceStore::supportFilePath).
	std::string supportFileProduct(const s100::sdb::SupportFileRecord& supportFile)
	{
		const std::string& path = supportFile.path;
		const auto first = path.find('/');
		const auto second = first == std::string::npos ? std::string::npos : path.find('/', first + 1);
		if (second == std::string::npos || second == first + 1)
		{
			return unknownProduct;
		}
		return path.substr(first + 1, second - first - 1);
	}

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
	DDX_Control(pDX, IDC_COMBO_DATASET_PRODUCT, m_comboProduct);
}


BEGIN_MESSAGE_MAP(DATASET_SUPPORTdlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &DATASET_SUPPORTdlg::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &DATASET_SUPPORTdlg::OnNMRClickList2)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD, &DATASET_SUPPORTdlg::OnMenuLoad)
	ON_COMMAND(ID_FILE_LOADFOLDER, &DATASET_SUPPORTdlg::OnMenuLoadFolder)
	ON_COMMAND(ID_FILE_LOADPROTECTED, &DATASET_SUPPORTdlg::OnMenuLoadProtected)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE32778, &DATASET_SUPPORTdlg::OnMenuDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENFOLDER, &DATASET_SUPPORTdlg::OnMenuOpenFolder)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &DATASET_SUPPORTdlg::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &DATASET_SUPPORTdlg::OnNMCustomdrawList2)
	ON_CBN_SELCHANGE(IDC_COMBO_DATASET_PRODUCT, &DATASET_SUPPORTdlg::OnCbnSelchangeComboProduct)
END_MESSAGE_MAP()


BOOL DATASET_SUPPORTdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_list1.InsertColumn(DatasetName, _T("Name"), LVCFMT_LEFT, 115);
	m_list1.InsertColumn(DatasetProduct, _T("Product"), LVCFMT_LEFT, 55);
	m_list1.InsertColumn(DatasetEdition, _T("Edition"), LVCFMT_LEFT, 55);
	m_list1.InsertColumn(DatasetIssued, _T("Issued"), LVCFMT_LEFT, 80);
	m_list1.InsertColumn(DatasetStatus, _T("Status"), LVCFMT_LEFT, 60);
	m_list1.InsertColumn(DatasetPath, _T("FilePath"), LVCFMT_LEFT, 275);
	m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_list2.InsertColumn(SupportName, _T("Name"), LVCFMT_LEFT, 135);
	m_list2.InsertColumn(SupportProduct, _T("Product"), LVCFMT_LEFT, 55);
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
	auto datasets = database.datasets().all();
	auto supportFiles = database.supportFiles().all();
	FillProductCombo(datasets, supportFiles);

	m_vecDS.clear();
	m_vecSP.clear();
	for (auto& dataset : datasets)
	{
		if (m_product.empty() || datasetProduct(dataset) == m_product)
		{
			m_vecDS.push_back(std::move(dataset));
		}
	}
	for (auto& supportFile : supportFiles)
	{
		if (m_product.empty() || supportFileProduct(supportFile) == m_product)
		{
			m_vecSP.push_back(std::move(supportFile));
		}
	}

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
		m_list1.SetItemText(row, DatasetProduct, DatasetManagerUi::Widen(datasetProduct(dataset)).c_str());
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
		m_list2.SetItemText(row, SupportProduct, DatasetManagerUi::Widen(supportFileProduct(supportFile)).c_str());
		m_list2.SetItemText(row, SupportPath, session->store().absolute(supportFile.path).c_str());
	}
}

void DATASET_SUPPORTdlg::FillProductCombo(const std::vector<s100::sdb::DatasetRecord>& datasets,
	const std::vector<s100::sdb::SupportFileRecord>& supportFiles)
{
	struct Counts
	{
		std::size_t datasets = 0;
		std::size_t supportFiles = 0;
	};
	std::map<std::string, Counts> products;
	for (const auto& dataset : datasets)
	{
		++products[datasetProduct(dataset)].datasets;
	}
	for (const auto& supportFile : supportFiles)
	{
		++products[supportFileProduct(supportFile)].supportFiles;
	}

	// A product that is no longer installed falls back to all products.
	if (!m_product.empty() && !products.contains(m_product))
	{
		m_product.clear();
	}

	m_products.clear();
	m_comboProduct.ResetContent();
	m_comboProduct.AddString(std::format(L"All products ({} datasets, {} support files)",
		datasets.size(), supportFiles.size()).c_str());
	int selection = 0;
	for (const auto& [product, counts] : products)
	{
		m_products.push_back(product);
		m_comboProduct.AddString(std::format(L"{} ({} datasets, {} support files)",
			DatasetManagerUi::Widen(product), counts.datasets, counts.supportFiles).c_str());
		if (product == m_product)
		{
			selection = static_cast<int>(m_products.size());
		}
	}
	m_comboProduct.SetCurSel(selection);
}

void DATASET_SUPPORTdlg::OnCbnSelchangeComboProduct()
{
	const int selection = m_comboProduct.GetCurSel();
	m_product = selection > 0 && static_cast<std::size_t>(selection) <= m_products.size()
		? m_products[static_cast<std::size_t>(selection) - 1] : std::string();
	SetRefreshItem();
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

void DATASET_SUPPORTdlg::OnMenuLoadFolder()
{
	CFolderPickerDialog dlg(nullptr, OFN_ALLOWMULTISELECT, this);
	if (dlg.DoModal() == IDOK)
	{
		std::vector<std::filesystem::path> folders;
		for (POSITION position = dlg.GetStartPosition(); position != nullptr;)
		{
			folders.emplace_back(dlg.GetNextPathName(position).GetString());
		}
		LoadFiles(folders);
	}
}

void DATASET_SUPPORTdlg::OnMenuLoadProtected()
{
	LoadProtected({}, {});
}

void DATASET_SUPPORTdlg::LoadProtected(const std::filesystem::path& source, const std::filesystem::path& permitFile)
{
	auto* session = DatasetManagerUi::Session(this);
	if (!session)
	{
		return;
	}
	ProtectedDataDlg dialog(*session, this);
	dialog.source = source;
	dialog.permitFile = permitFile;
	if (dialog.DoModal() != IDOK || !dialog.report())
	{
		return;
	}

	// An exchange set may bring catalogues as well as datasets.
	session->refreshCatalogues();
	session->refreshDatasets();
	SetRefreshItem();
	theApp.MapRefresh();
	theApp.m_pDockablePaneLayerManager.UpdateList();

	const auto& report = *dialog.report();
	const std::vector<std::string> notes{ std::format("Decrypted {} of {} protected files (HW_ID {}{}).", report.decryptedCount(),
		report.files.size(), report.hwId, report.dataServerName.empty() ? "" : ", data server " + report.dataServerName) };
	std::vector<std::string> problems;
	constexpr std::size_t listed = 8;
	std::size_t failed = 0;
	for (const auto& file : report.files)
	{
		if (!file.decrypted && failed++ < listed)
		{
			problems.push_back(file.file + ": not decrypted: " + file.message);
		}
	}
	if (failed > listed)
	{
		problems.push_back(std::format("... and {} more not decrypted", failed - listed));
	}
	if (!report.expiredPermits.empty())
	{
		const auto& first = report.expiredPermits.front();
		problems.push_back(std::format("{} permit(s) have expired (the data was installed all the same), e.g. {} on {}.",
			report.expiredPermits.size(), first.fileName, first.expiry));
	}
	for (std::size_t i = 0; i < report.warnings.size() && i < listed; ++i)
	{
		problems.push_back(report.warnings[i]);
	}
	problems.insert(problems.end(), report.import.installProblems.begin(), report.import.installProblems.end());
	DatasetManagerUi::Report(this, report.import.results, problems, notes);
}

void DATASET_SUPPORTdlg::LoadFiles(const std::vector<std::filesystem::path>& files)
{
	auto* session = DatasetManagerUi::Session(this);
	if (!session)
	{
		return;
	}

	// Protected data cannot be installed as it is: its files only read once decrypted.
	std::vector<std::filesystem::path> plain;
	for (const auto& file : files)
	{
		if (const auto permit = FindPermitFor(file))
		{
			LoadProtected(file, *permit);
		}
		else
		{
			plain.push_back(file);
		}
	}
	if (plain.empty())
	{
		return;
	}
	LoadPlainFiles(plain);
}

void DATASET_SUPPORTdlg::LoadPlainFiles(const std::vector<std::filesystem::path>& files)
{
	auto* session = DatasetManagerUi::Session(this);
	if (!session)
	{
		return;
	}

	CWaitCursor wait;
	std::vector<s100::sdb::ImportResult> results;
	std::vector<std::string> problems;
	const auto add = [&](const s100::sdb::ImportReport& report) {
		results.insert(results.end(), report.results.begin(), report.results.end());
		problems.insert(problems.end(), report.installProblems.begin(), report.installProblems.end());
	};
	for (const auto& file : files)
	{
		std::error_code error;
		if (std::filesystem::is_directory(file, error))
		{
			add(session->importer().importFolder(file));
		}
		else if (isExchangeCatalogue(file))
		{
			add(session->importer().importExchangeSet(file));
		}
		else if (session->importer().acceptsDataset(file))
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
