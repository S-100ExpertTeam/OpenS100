#include "pch.h"
#include "PropertyGridAttributeModify.h"
#include "resource.h"
#include "DockablePaneEditWindow.h"
#include "OpenS100.h"
#include "OpenS100View.h"

#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\GISLibrary\\S101Layer.h"
#include "..\\GISLibrary\\F_ATTR.h"
#include "..\\GISLibrary\\S101Cell.h"
#include "..\\GISLibrary\\S100Layer.h"
#include "..\\GISLibrary\\R_InformationRecord.h"
#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\GISLibrary\\ATTR.h"
#include "..\\GISLibrary\\CodeWithNumericCode.h"
#include "..\\FeatureCatalog\\AttributeBinding.h"


#include <string.h>
// collection of functions that define the behavior of the feature information list.
// CPropertyModify

IMPLEMENT_DYNAMIC(CPropertyGridAttributeModify, CMFCPropertyGridCtrl)

CPropertyGridAttributeModify::CPropertyGridAttributeModify()
{
	pSelectedProperty = NULL;
}

CPropertyGridAttributeModify::~CPropertyGridAttributeModify()
{
}


BEGIN_MESSAGE_MAP(CPropertyGridAttributeModify, CMFCPropertyGridCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


S101Cell* CPropertyGridAttributeModify::GetS101Cell()
{
	return (S101Cell*)spatialObject;
}


void CPropertyGridAttributeModify::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty *pProp = HitTest(point, &clickArea);

		if (clickArea == CMFCPropertyGridProperty::ClickArea::ClickValue)
		{
			return;
		}
		CMFCPropertyGridCtrl::OnLButtonDown(nFlags, point);
		return;

	CMFCPropertyGridCtrl::OnLButtonDown(nFlags, point);
}

void CPropertyGridAttributeModify::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty* pHit = HitTest(point, &clickArea);

		return;
}
void CPropertyGridAttributeModify::OnDeleteAttribute()
{
	CWnd* pParent = GetParent(); // Bring the parent's value of the selected value.
	if (pSelectedProperty) // The selected value.
	{
		// Bring information about the selected data.
		MultiData *multiData = (MultiData *)pSelectedProperty->GetData();
		if (multiData == nullptr)
		{
			return;
		}
		ATTR *attr = (ATTR *)multiData->data[2];

		CString name = pSelectedProperty->GetName();

		// not to use the name for ATTR.
		auto s101Cell = GetS101Cell();
		auto DGIR = s101Cell->GetDatasetGeneralInformationRecord();
		auto attributeCode = DGIR->GetAttributeCode(attr->m_natc);
	}
}
void CPropertyGridAttributeModify::SetSpatialObject(SpatialObject *spatialObject)
{
	this->spatialObject = spatialObject;
}
void CPropertyGridAttributeModify::ComplexAttriIndexnumSet(int num)
{
	this->ComplexAttriIndexnum = num;
}
int CPropertyGridAttributeModify::ComplexAttriIndexnumGet()
{
	return ComplexAttriIndexnum;
}
void CPropertyGridAttributeModify::OnAttributeeditOpen()
{
	if (pSelectedProperty != nullptr) //If the selected value is not NULL,
	{
		CString Value = pSelectedProperty->GetValue();
		ShellExecute(NULL, _T("open"), _T("iexplore"), NULL, NULL, SW_SHOW); //Open the Explorer.
	}
}
void CPropertyGridAttributeModify::OnAtttributeeditDelete() //delete Attribute
{
	OnDeleteAttribute();
}

void CPropertyGridAttributeModify::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty *pProp = HitTest(point, &clickArea);

	if (clickArea == CMFCPropertyGridProperty::ClickArea::ClickExpandBox)
	{
		CMFCPropertyGridCtrl::OnLButtonDblClk(nFlags, point);
		return;
	}
}