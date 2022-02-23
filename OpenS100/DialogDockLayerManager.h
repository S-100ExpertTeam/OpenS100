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

	CMFCPropertyGridCtrl m_wndPropList;
	CFont m_fntPropList;

	CDWordArray m_anDragIndexes; // Remember the item you chose.
	std::vector<CString> SelectdItem;

	bool isclickLayerManager = false; //Layer Manager Property Click Limit

public:
	void AdjustLayout();
	void InitPropList();
	void RemoveAllPropList();
	void DeleteLayer();
	void FillPropList(S101Cell*);
	void SetPropListFont();
	void UpdateList();
	void FocusLayerRange();
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnButtonDelete();
};