#pragma once
#include "resource.h"
#include <vector>

class CConfigrationDlg;
// The dialog displayed on the first tab of the setting window.
class CConfiguration_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CConfiguration_1)

public:
	CConfiguration_1(CWnd* pParent = NULL);
	virtual ~CConfiguration_1();
	
public:
	CConfigrationDlg *m_pParent = nullptr;
	enum { IDD = IDD_DIALOG_CONFIG_1 };

	int m_displayMode = 0;
	int m_colorScheme = 0; //The number of the selected svg file.
	int m_SymbolizedAreaBoundary = 0;

	std::vector<CString> m_fontList;
	CButton m_twoShade;
	CButton m_useNationalObjectName;
	CButton checkBoxIgnoreScaleMin;
	CButton checkBoxShallowPattern;
	CButton checkBoxSimplifyPoint;
	CButton checkBoxShowIsolatedDanger;
	CButton checkBoxSimplifyLine;
	CButton checkBoxFullSector;
	CButton showinform01;
	CEdit	m_safetyDepth;
	CEdit	m_safetyWaterLevel;
	CEdit	m_veryShallowWaterLevel;
	CEdit	m_veryDeepWaterLevel;

public:
	void SetFontList(std::vector<CString> fontList);
	std::vector<CString>* GetFontList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
};