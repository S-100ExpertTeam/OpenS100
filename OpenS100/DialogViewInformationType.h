#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "InformationCodeString.h"

#include "../GISLibrary/S100SpatialObject.h"

#include <vector>

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
	void SetInformationFeatureList(S100SpatialObject* s100Dataset);
	std::vector<CInformationCodeString> ngflist;
	void InitInformationFeatureList();
	S100SpatialObject* s100Dataset = nullptr;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ViewListInformationType;
	afx_msg void OnBnClickedOk();
};