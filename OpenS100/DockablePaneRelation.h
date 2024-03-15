#pragma once

#include "../GISLibrary/S100SpatialObject.h"
#include "../GISLibrary/GF_FeatureType.h"
#include "../GISLibrary/GF_InformationType.h"

class CDialogDockRelation;

class CDockablePaneRelation : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneRelation)

private:
	CDialogDockRelation* pDlg = nullptr;

public:
	void UpdateList();
	void SetFeatureList(S100SpatialObject* cell, std::list<GF::FeatureType*> flist, std::list<GF::InformationType*> infoList);

	BOOL CanBeClosed() const;
	BOOL CanBeResized() const;
	BOOL CanFloat() const;
	BOOL CanAutoHide() const;

	CDockablePaneRelation();
	virtual ~CDockablePaneRelation();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	virtual void OnFinalRelease();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};