#pragma once
#include "Configuration_1.h"
#include "Configuration_4.h"

#include "../GeoMetryLibrary/ENCConfiguration.h"

class CGISViewerView;
class FeatureCatalogue;
class CConfigrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigrationDlg)

public:
	CConfigrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigrationDlg();

public:
// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG };

	CConfiguration_1 mainTab;
	CConfiguration_4 s101Tab;
	
	CTabCtrl m_tab;

	ENCConfiguration m_config;

	bool bObjectDisplaySettingChanges;
	
public:
	void SettingLoadFromFile(std::wstring filename);
	void InitS101FeatureTypes(FeatureCatalogue* catalog);
	void InitControlValues();
	

	CConfiguration_1* GetConfigPage1();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	afx_msg void OnTcnSelchangeConfigTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingConfigTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedApply();
};