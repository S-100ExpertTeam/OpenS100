#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

#include <list>

class CDialogDockRelation;
class COpenS100View;
class R_FeatureRecord;
class R_InformationRecord;
class S101Cell;

class CDialogDockRelation : public CDialog
{
	DECLARE_DYNAMIC(CDialogDockRelation)

public:
	CDialogDockRelation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogDockRelation();
	virtual BOOL OnInitDialog();

	void UpdateList();
	 
	// Dialog Data 
	enum { IDD = IDD_DIALOG_DOCK_RELATION };

	int nSelectedItem;
	S101Cell* m_cell;

public:
	void AdjustLayout();
	void SetFeatureList(S101Cell* cell, std::list<R_FeatureRecord*> flist, std::list<R_InformationRecord*> infoList);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnNMClickListLm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListLm(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ListRelation;
};