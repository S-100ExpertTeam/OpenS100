#pragma once
#include "afxdialogex.h"


// CDlg_Portrayal 대화 상자

class CDlg_Portrayal : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Portrayal)

public:
	CDlg_Portrayal(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Portrayal();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlg_Portrayal };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void AdjustLayout();
	void InitPortrayalCataloguePropertyGrid();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CListCtrl listPC;
	CMFCPropertyGridCtrl portrayalCataloguePropertyGrid;
};
