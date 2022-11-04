#pragma once
#include "afxdialogex.h"


// CDialogExFeatureInformationList 대화 상자

class CDialogExFeatureInformationList : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogExFeatureInformationList)

public:
	CDialogExFeatureInformationList(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogExFeatureInformationList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDialogExFeatureInformationList };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboType;
	CComboBox comboGeom;
	CListCtrl listCtrl;
	virtual BOOL OnInitDialog();
};
