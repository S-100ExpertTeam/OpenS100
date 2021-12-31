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
	afx_msg void OnBnClickedButton_initialization();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_viewingGroupList; // express the viewing group that I read on PC.
};