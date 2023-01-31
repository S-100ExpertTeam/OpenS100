#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

#include "../GISLibrary/S100SpatialObject.h"

#include <vector>
#include <map> 
#include <string>

class CurrentSelection;
class COpenS100View;
class R_FeatureRecord;
class InformationType;
class S101Cell;
class FeatureType;
class CGISLibraryApp;

class CDialogDockCurrentSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDockCurrentSelection)

public:
	CDialogDockCurrentSelection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogDockCurrentSelection();

	virtual BOOL OnInitDialog();

	// Dialog Data 
	enum { IDD = IDD_DIALOG_DOCK_CURRENT_SELECTION };

	int nSelectedItem = -1;
	CListCtrl m_ListCurrentSelection;
	S100SpatialObject* m_Cell = nullptr;

public:
	// Adjust column size of list control.
	void AdjustLayout();

	void StringSplit(std::string strTarget, std::string strTok, std::vector<std::string>& strResult);

	void UpdateList();
	void OnMenuRemoveAll();
	void DeleteItem(CString id);
	void UpdateListTest(CStringArray *csa, S101Cell *cell, CString isCtrlClicked);
	long long GetSelectedRecordName();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	// A function that occurs when you click on the list.
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};