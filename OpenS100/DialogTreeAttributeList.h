#pragma once
#include "TreeCtrlExt.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\FeatureCatalog\\AttributeBinding.h"
// DialogTreeAttributeList

class CGeoObject;
class FeatureType;
class InformationType;
class CDialogTreeAttributeList : public CTreeCtrlExt
{
	DECLARE_DYNAMIC(CDialogTreeAttributeList)

public:
	CDialogTreeAttributeList();
	virtual ~CDialogTreeAttributeList();

	/*
	//	1 : S-57
	//	2 :	S-100
	*/
	int m_type;
	int selectIndex;
	CMFCPropertyGridProperty *pParentAttribute = nullptr;
	int selectIndexNum = 0;
	
public:
	void SetShowTree(CGeoObject* ot);
	void SetShowTree(FeatureCatalogue* _fc, FeatureType* ft);
	void SetShowTree(FeatureCatalogue* _fc, InformationType* ift);
	void InsertAttribute(HTREEITEM &item);

	void ComplexSetShowTree(FeatureCatalogue* _catalog, AttributeBinding* ab); //basic setting.
	void BaseSetShowThee(); //If there's something that's already been added,

protected:	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};