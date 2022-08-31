#pragma once
/* 
 * Developed by Huzifa Terkawi 
 * http://www.codeproject.com/Members/huzifa30
 * All permission granted to use this code as long as you retain this notice
 * and refere to orginal Material when part of this code is re-puplished
*/
#include "PropertyGridAttributeModify.h"
#include "PropertyGridVectorModify.h"
#include "PropertyGridDrawingCommands.h"
#include "MFCTabCtrlMsgToParent.h"

#include "../FeatureCatalog/ListedValue.h"

#include <unordered_map>
#include <list>

class SpatialObject;
class R_FeatureRecord;
class R_InformationRecord;
class R_VectorRecord;
class R_PointRecord;
class R_MultiPointRecord;
class R_CurveRecord;
class R_CompositeRecord;
class R_SurfaceRecord;
class CAttribute;
class COpenS100View;
class AttributeBinding;
class SimpleAttribute;
class ComplexAttribute;
class EdtVector;
class Item;

// CFeatureView
/*

Type 001
arg 0 - CMFCPropertyGridProperty *
arg 1 - ENC_Vector*
arg 2 - SG2D*
arg 3 - ENC_Vector*

Type 011
arg 0 - CMFCPropertyGridProperty *
arg 1 - ATTF*

Type 012
arg 0 - CMFCPropertyGridProperty *
arg 1 - NATF*

Type 100
arg 0 - CMFCPropertyGridProperty *
arg 1 - R_VectorRecord*
arg 2 - IC2D*
arg 3 - F_C2IL*
arg 4 - int (index)

Type 101
arg 0 - CMFCPropertyGridProperty *
arg 1 - R_VectorRecord*
arg 2 - F_C2IT*

Type 102
arg 0 - CMFCPropertyGridProperty *
arg 1 - R_VectorRecord*
arg 2 - IC3D*
arg 3 - F_C3IL*
arg 4 - int (index)

Type 111
arg 0 - CMFCPropertyGridProperty *
arg 1 - R_FeatureRecord*
arg 2 - ATTR*
arg 3 - Type (0 : Feature, 1 : Info)

Type 200
arg 0 - CMFCPropertyGridProperty *
arg 1 - F_INAS*
arg 2
*/

class MultiData
{
public:
	int type;
	std::vector<DWORD_PTR> data;
};

// Feature/Information Type's information dialog for pick report.
class CDockablePaneEditWindow : public CDockablePane
{
	DECLARE_DYNAMIC(CDockablePaneEditWindow)
	
public:
	CDockablePaneEditWindow();
	virtual ~CDockablePaneEditWindow();

private:
	CImageList			m_imageList;
	CFont				m_fntPropList;
	CPropertyGridVectorModify			m_wndListVector;
	CPropertyGridDrawingCommands		m_wndDrawingCommands;

public:
	SpatialObject *m_cell = nullptr;
	CMFCTabCtrlMsgToParent	m_wndTabs;

	static CPropertyGridAttributeModify		m_wndListAttribute;

	int m_vIndex;

	// S-100
	R_FeatureRecord* m_pFeature;
	R_InformationRecord* m_pInformation;
	MultiData* m_selectedMultiData;

	std::unordered_map<CMFCPropertyGridProperty*, MultiData*> m_propertyAttributeMultiData;
	std::unordered_map<CMFCPropertyGridProperty*, MultiData*> m_propertyVectorMultiData;

	CString m_selectedObjectType;
	CString m_selectedInformationName;

public:
	void SetFeatureRecord(R_FeatureRecord* pFeature);
	void SetFeatureRecord(R_InformationRecord* pInformation);
	void SetAttributes();
	void SetVectors();
	void SetVector(int RCNM, R_VectorRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(R_PointRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(R_MultiPointRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(R_CurveRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(R_CompositeRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(R_SurfaceRecord* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void ClearVector();

	void SetFeatureList();
	void DeleteAllItems();
	void DeleteAttributeItems();
	void DeleteVectorItems();

	//// Sort and then add the enumtype to the Simple Attribute.
	void SettingEnumType(std::vector<ListedValue*> list, CMFCPropertyGridProperty* pSuperProperty);
	//  
	unsigned GetATCS(std::wstring code);
	unsigned GetATIX(unsigned natc, unsigned parentIndex);

	void SetSpatialObject(SpatialObject *object);
	SpatialObject *GetSpatialObject();
	S101Cell* GetS101Cell();

	CString GetActiveTabName();
	bool IsAttributeTab();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lp);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP();

	MultiData* InsertPropertyMultiData(
		int multidataType, 
		CMFCPropertyGridProperty* pGP, 
		DWORD_PTR pointer_1, 
		DWORD_PTR pointer_2 = NULL, 
		DWORD_PTR pointer_3 = NULL, 
		DWORD_PTR pointer_4 = NULL);

	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};