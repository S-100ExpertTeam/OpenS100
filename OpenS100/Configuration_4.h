#pragma once
#include "resource.h"

#include <unordered_map>
#include <string>

class CConfigrationDlg;
class S100_Description;
// Dialogue displayed on the S-101 tab of the setting window.
class CConfiguration_4 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguration_4)

public:
	CConfiguration_4(CWnd* pParent = nullptr);   
	virtual ~CConfiguration_4();

public:
	enum { IDD = IDD_DIALOG_CONFIG_4 };
	CConfigrationDlg* m_pParent = nullptr;

private :
	std::unordered_map<std::wstring, S100_Description*> viewing_map;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CListCtrl m_viewingGroupList;
	CComboBox comboBoxProduct;
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedButtonUncheck();
	virtual void OnCancel();
	virtual void OnOK();
};