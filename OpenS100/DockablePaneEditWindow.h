#pragma once
/* 
 * Developed by Huzifa Terkawi 
 * http://www.codeproject.com/Members/huzifa30
 * All permission granted to use this code as long as you retain this notice
 * and refere to orginal Material when part of this code is re-puplished
*/
#include "PropertyGridVectorModify.h"
#include "PropertyGridDrawingCommands.h"
#include "MFCTabCtrlMsgToParent.h"

#include "../FeatureCatalog/ListedValue.h"

#include "../GISLibrary/GM_Object.h"
#include "../GISLibrary/GM_Point.h"
#include "../GISLibrary/GM_MultiPoint.h"
#include "../GISLibrary/GM_Curve.h"
#include "../GISLibrary/GM_CompositeCurve.h"
#include "../GISLibrary/GM_Surface.h"
#include "../GISLibrary/SPoint.h"
#include "../GISLibrary/SMultiPoint.h"
#include "../GISLibrary/SCurve.h"
#include "../GISLibrary/SCompositeCurve.h"
#include "../GISLibrary/SSurface.h"

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
class S100SpatialObject;

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
arg 2 - C2IL*
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
	S100SpatialObject *m_cell = nullptr;
	CMFCTabCtrlMsgToParent	m_wndTabs;

	static CPropertyGridVectorModify	m_wndListAttribute;

	// S-100
	//R_FeatureRecord* m_pFeature;
	//R_InformationRecord* m_pInformation;
	std::wstring selectedFeatureID;
	std::wstring selectedInformationID;

	CString m_selectedObjectType;

public:
	void SetFeatureType(std::wstring featureID);
	void SetInformationType(std::wstring informationID);
	void SetAttributes();
	void SetVectors();
	
	void SetVector(SGeometry* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void SetVector(GM::Point* r, CMFCPropertyGridProperty *pSuperProperty = nullptr);
	void SetVector(SPoint* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void SetVector(GM::MultiPoint* r, CMFCPropertyGridProperty* pSuperProperty = nullptr);
	void SetVector(SMultiPoint* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void SetVector(GM::Curve* r, CMFCPropertyGridProperty* pSuperProperty = nullptr);
	void SetVector(SCurve* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void SetVector(GM::CompositeCurve* r, CMFCPropertyGridProperty* pSuperProperty = nullptr);
	void SetVector(SCompositeCurve* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void SetVector(GM::Surface* r, CMFCPropertyGridProperty* pSuperProperty = nullptr);
	void SetVector(SSurface* geom, CMFCPropertyGridProperty* pSuperProperty = nullptr);

	void ClearVector();

	void SetFeatureList();
	void DeleteAllItems();
	void DeleteAttributeItems();
	void DeleteVectorItems();

	//// Sort and then add the enumtype to the Simple Attribute.
	void SettingEnumType(std::vector<ListedValue*> list, CMFCPropertyGridProperty* pSuperProperty);

	void SetSpatialObject(S100SpatialObject *object);
	//SpatialObject *GetSpatialObject();
	//S101Cell* GetS101Cell();

	//CString GetActiveTabName();
	//bool IsAttributeTab();

	void addSimpleAttribute(CMFCPropertyGridProperty* parent, std::string code, std::string value, FeatureCatalogue* fc);
	void addComplexAttribute(CMFCPropertyGridProperty* parent, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lp);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP();

	//afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};