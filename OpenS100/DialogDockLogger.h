#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

class CDialogDockLogger;
class COpenS100View;
class R_FeatureRecord;
class S101Cell;
// CDialogDockLogger dialog
class CDialogDockLogger : public CDialog
{ 
	DECLARE_DYNAMIC(CDialogDockLogger)

public:
	CDialogDockLogger(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogDockLogger();

public:
	// Dialog Data 
	enum { IDD = IDD_DIALOG_DOCK_LOGGER };
	COpenS100View *pView;
	int nSelectedItem;

	static CListCtrl m_ListLogger;
	static CComboBox m_ComboLogType; //static combo box

	CButton b_ListClearButton;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};