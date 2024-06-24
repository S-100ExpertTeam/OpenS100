#pragma once
#include "afxdialogex.h"
#include "ConfigrationDlg.h"
#include "DataSetManagerSupport.h"



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
	CListCtrl m_list1;
	CListCtrl m_list2;

	std::vector<std::shared_ptr<DatasetClass>> m_vecDS;
	std::vector<std::shared_ptr<DatasetClass>> m_vecSP;

	void OpenFolder(CListCtrl& listControl, std::vector<std::shared_ptr<DatasetClass>>& vec);
	void LoadFile(CString filePath);
	void DeleteSelectedItems(CListCtrl& listControl, std::vector<std::shared_ptr<DatasetClass>>& vec);
	void Set_Item(std::shared_ptr<DatasetClass> item);
	void SetRefreshItem();



	virtual BOOL OnInitDialog();

	void OnMenuLoad(CCmdUI* pCmdUI);
	void OnMenuDelete(CCmdUI* pCmdUI);
	void OnMenuOpenFolder(CCmdUI* pCmdUI);

	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);
};
