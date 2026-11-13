#pragma once
#include "afxdialogex.h"
#include "ConfigrationDlg.h"

#include "../SystemDatabase/Records.h"

#include <filesystem>
#include <vector>

// Dataset Manager tab: the datasets and support files installed in the System Database.
// Loading installs (an exchange set's CATALOG.XML, a dataset or update file, a support file, or
// a whole folder of them; a file delivered twice is installed once); deleting removes. The chart
// redraws from the database; nothing is kept in the dialog itself.
class DATASET_SUPPORTdlg : public CDialogEx
{
	DECLARE_DYNAMIC(DATASET_SUPPORTdlg)

public:
	DATASET_SUPPORTdlg(CWnd* pParent = nullptr);
	virtual ~DATASET_SUPPORTdlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATASET_SUPPORT };
#endif

	CConfigrationDlg* m_pParent = nullptr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()


public:
	CListCtrl m_list1;   // datasets
	CListCtrl m_list2;   // support files
	CComboBox m_comboProduct;   // product specification filter: "All" first, then one entry per product

	// The rows of the two lists, in list order (only the selected product's, when one is selected).
	std::vector<s100::sdb::DatasetRecord> m_vecDS;
	std::vector<s100::sdb::SupportFileRecord> m_vecSP;

	// Protected data among `files` (a folder, CATALOG.XML or .zip with a PERMIT.XML) goes to the
	// Load Protected Data dialog, one at a time; the rest is installed as it is.
	void LoadFiles(const std::vector<std::filesystem::path>& files);
	// Asks for PERMIT.XML and the credentials and installs S-100 Part 15 protected data.
	void LoadProtected(const std::filesystem::path& source, const std::filesystem::path& permitFile);
	void SetRefreshItem();

	virtual BOOL OnInitDialog();

	void OnMenuLoad(CCmdUI* pCmdUI);
	afx_msg void OnMenuLoadFolder();
	afx_msg void OnMenuLoadProtected();
	void OnMenuDelete(CCmdUI* pCmdUI);
	void OnMenuOpenFolder(CCmdUI* pCmdUI);

	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboProduct();

private:
	void LoadPlainFiles(const std::vector<std::filesystem::path>& files);

	// The product specifications in the database, in combo order after "All"; the selected one,
	// or empty for all products.
	std::vector<std::string> m_products;
	std::string m_product;

	void FillProductCombo(const std::vector<s100::sdb::DatasetRecord>& datasets,
		const std::vector<s100::sdb::SupportFileRecord>& supportFiles);
	void ShowContextMenu();
	// The selected rows of the focused list, and whether it is the dataset list.
	std::vector<std::size_t> SelectedRows(bool& datasets);
	void HighlightMissing(CListCtrl& list, int pathColumn, NMHDR* pNMHDR, LRESULT* pResult);
};
