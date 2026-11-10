#pragma once
#include "afxdialogex.h"
#include "ConfigrationDlg.h"

#include "../SystemDatabase/Records.h"

#include <filesystem>
#include <vector>

class CatalogManager;

// Dataset Manager tab: the feature and portrayal catalogues installed in the System Database.
// Loading a file installs it; deleting removes it. Nothing is kept in the dialog itself.
class FC_PCSelectdlg : public CDialogEx
{
	DECLARE_DYNAMIC(FC_PCSelectdlg)

public:
	FC_PCSelectdlg(CWnd* pParent = nullptr);
	virtual ~FC_PCSelectdlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FC_PC };
#endif

	CConfigrationDlg* m_pParent = nullptr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:

	CatalogManager* ct = nullptr;

	CListCtrl m_list1;   // feature catalogues
	CListCtrl m_list2;   // portrayal catalogues

	// The rows of the two lists, in list order.
	std::vector<s100::sdb::CatalogueRecord> m_vecFC;
	std::vector<s100::sdb::CatalogueRecord> m_vecPC;

	virtual BOOL OnInitDialog();

	// Installs feature catalogues (their XML) and portrayal catalogues (their folder or the
	// portrayal_catalogue.xml in it).
	void LoadFiles(const std::vector<std::filesystem::path>& files);
	void SetRefreshItem();

	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);

	void OnMenuLoad(CCmdUI* pCmdUI);
	void OnMenuDelete(CCmdUI* pCmdUI);
	void OnMenuOpenFolder(CCmdUI* pCmdUI);

	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);

private:
	void ShowContextMenu();
	void FillList(CListCtrl& list, const std::vector<s100::sdb::CatalogueRecord>& records);
	// The selected rows of the focused list, and which list that is.
	std::vector<s100::sdb::CatalogueRecord> SelectedRecords(bool& featureCatalogues);
	void HighlightMissing(CListCtrl& list, NMHDR* pNMHDR, LRESULT* pResult);
};
