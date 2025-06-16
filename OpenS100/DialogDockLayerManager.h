#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

#include <vector>

class LayerManager;
class COpenS100View;
class R_FeatureRecord;
class S101Cell;
class S413Cell;

class CDialogDockLayerManager : public CDialog
{
	DECLARE_DYNAMIC(CDialogDockLayerManager)

public:
	CDialogDockLayerManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogDockLayerManager();

public:
	// Dialog Data
	enum { IDD = IDD_DIALOG_DOCK_LAYERMANEGER };

	CFont m_fntPropList;

	CDWordArray m_anDragIndexes; // Remember the item you chose.
	std::vector<CString> SelectdItem;

	bool isclickLayerManager = false; //Layer Manager Property Click Limit

	CListCtrl listCtrlLayers;
	CMFCPropertyGridCtrl* propertyGridLayer;

public:
	void AdjustLayout();
	void InitListCtrl();
	void InitPropList();

	void RemoveAll();
	void RemoveAllListCtrl();
	void RemoveAllPropList();

	void DeleteLayer();
	void FillPropList(S101Cell*);
	void SetPropListFont();
	void UpdateList();
	void FocusLayerRange();
	virtual BOOL OnInitDialog();

	int GetSelectedLayerIndex();

	S101Cell* GetSelectedCell();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnButtonDelete();
public:
	afx_msg void OnItemchangedListLm(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
};