#pragma once
#include "afxdialogex.h"
#include "ConfigrationDlg.h"

#include "DataSetManagerSupport.h"




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
	CListCtrl m_list1;
	CListCtrl m_list2;

	std::vector<std::shared_ptr<FC_PC_DatasetClass>> m_vecFC;
	std::vector<std::shared_ptr<FC_PC_DatasetClass>> m_vecPC;


	virtual BOOL OnInitDialog();

	void DeleteSelectedItems(CListCtrl& listControl, std::vector<std::shared_ptr<FC_PC_DatasetClass>>& vec);

	void LoadFile(CString filePath);
	void SetFC_PC_Item(std::shared_ptr<FC_PC_DatasetClass> item);
	void SetRefreshItem();

	std::vector<int> parseVersion(const std::string& version);
	bool isValidVersion(const std::string& version);
	int compareVersions(const std::string& v1, const std::string& v2);
	std::string chooseVersion(const std::string& current, const std::string& newVersion);


	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);

	void OnMenuLoad(CCmdUI* pCmdUI);
	void OnMenuDelete(CCmdUI* pCmdUI);
	void OnMenuOpenFolder(CCmdUI* pCmdUI);
	void OpenFolder(CListCtrl& listControl, std::vector<std::shared_ptr<FC_PC_DatasetClass>>& vec);

	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);
};
