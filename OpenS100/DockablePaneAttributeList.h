#pragma once
#include "DialogTreeAttributeList.h"

#include "..\\featureCatalog\\AttributeBinding.h"
#define  IDC_BT_INSERT  150
// CDOperator

class CGeoObject;
class FeatureType;
class InformationType;

class CDockablePaneAttributeList : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneAttributeList)

public:
	CDockablePaneAttributeList();
	virtual ~CDockablePaneAttributeList();
	void AbjustLayout();

public:
	CDialogTreeAttributeList m_treeCtrl;
	CButton m_button;

public:
	void SetShowTree(CGeoObject* ot);
	void SetShowTree(FeatureCatalogue* fc, FeatureType* ft);
	void SetShowTree(FeatureCatalogue* fc, InformationType* ift);
	void ComplexSetShowTree(FeatureCatalogue* fc, AttributeBinding* ab);
	void BaseSetShowThee();

	void SetParentProperty(CMFCPropertyGridProperty *pParentAttribute);
	void SetindexNum(int indexNum);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); //creat attribute List 
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateButton(CCmdUI *pCmdUI);
	afx_msg void RemoveAll(); 
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};