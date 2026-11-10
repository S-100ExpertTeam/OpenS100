#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

#include <vector>

class Layer;
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
	// Lists the layers of the layer manager. The System Database layer is not listed itself: the
	// datasets it draws now are listed in its place.
	void UpdateList();
	// UpdateList, when what is drawn has changed since the list was filled (after each draw).
	void RefreshIfChanged();
	void FocusLayerRange();
	virtual BOOL OnInitDialog();

	// The index in the layer manager of the selected row's layer; for a dataset of the System
	// Database, the index of the System Database layer. -1 when nothing is selected.
	int GetSelectedLayerIndex();
	// The selected row's layer (a System Database dataset itself), or null.
	Layer* GetSelectedLayer();
	// Whether the selected row is a dataset drawn from the System Database.
	bool IsSystemDatabaseDatasetSelected();

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

private:
	struct Row
	{
		int layerIndex = -1;   // in the layer manager
		int innerIndex = -1;   // in the System Database chart's layers; -1 for a layer of its own
		Layer* layer = nullptr;   // to notice changes only; resolved again on use
	};

	[[nodiscard]] std::vector<Row> CurrentRows() const;
	[[nodiscard]] Layer* Resolve(int row) const;
	void ShowProperties(Layer* layer);

	std::vector<Row> rows;
};