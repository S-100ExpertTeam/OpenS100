#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "InformationCodeString.h"

#include <vector>

class S101Cell;
class CDialogViewInformationType : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogViewInformationType)

public:
	CDialogViewInformationType(CWnd* pParent = nullptr);  
	virtual ~CDialogViewInformationType();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INFORMATIONTYPE };
#endif
public:
	void SetInformationFeatureList(S101Cell* cell);
	std::vector<CInformationCodeString> ngflist;
	void InitInformationFeatureList();
	S101Cell* m_cell;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ViewListInformationType;
	afx_msg void OnBnClickedOk();
};