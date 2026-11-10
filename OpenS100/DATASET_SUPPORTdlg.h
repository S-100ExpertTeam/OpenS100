#pragma once
#include "afxdialogex.h"
#include "ConfigrationDlg.h"

#include "../SystemDatabase/Records.h"

#include <filesystem>
#include <vector>

// Dataset Manager tab: the datasets and support files installed in the System Database.
// Loading installs (an exchange set's CATALOG.XML, a dataset or update file, a support file);
// deleting removes. The chart redraws from the database; nothing is kept in the dialog itself.
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

	// The rows of the two lists, in list order.
	std::vector<s100::sdb::DatasetRecord> m_vecDS;
	std::vector<s100::sdb::SupportFileRecord> m_vecSP;

	void LoadFiles(const std::vector<std::filesystem::path>& files);
	void SetRefreshItem();

	virtual BOOL OnInitDialog();

	void OnMenuLoad(CCmdUI* pCmdUI);
	void OnMenuDelete(CCmdUI* pCmdUI);
	void OnMenuOpenFolder(CCmdUI* pCmdUI);

	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void ShowContextMenu();
	// The selected rows of the focused list, and whether it is the dataset list.
	std::vector<std::size_t> SelectedRows(bool& datasets);
	void HighlightMissing(CListCtrl& list, int pathColumn, NMHDR* pNMHDR, LRESULT* pResult);
};
