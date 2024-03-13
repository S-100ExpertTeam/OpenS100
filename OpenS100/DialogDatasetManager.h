#pragma once
#include "afxdialogex.h"
#include "Configuration_1.h"
#include "Configuration_4.h"
#include "FC_PCSelectdlg.h"
#include "DATASET_SUPPORTdlg.h"


class DialogDatasetManager : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDatasetManager)

public:
	DialogDatasetManager(CWnd* pParent = nullptr);   
	virtual ~DialogDatasetManager();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DATASET_MANAGER };
#endif

	FC_PCSelectdlg mainTab;
	DATASET_SUPPORTdlg DSTab;

	CTabCtrl m_tab;

	CatalogManager* ct;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeConfigTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchangingConfigTab(NMHDR* pNMHDR, LRESULT* pResult);
};
