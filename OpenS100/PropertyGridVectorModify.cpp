#include "pch.h"
#include "PropertyGridVectorModify.h"
#include "resource.h"

#include "../GISLibrary/F_ATTR.h"
#include "..\\OpenS100\\OpenS100.h"

// CPropertyModify

IMPLEMENT_DYNAMIC(CPropertyGridVectorModify, CMFCPropertyGridCtrl)

CPropertyGridVectorModify::CPropertyGridVectorModify()
{
	pSelectedProperty = NULL;
}

CPropertyGridVectorModify::~CPropertyGridVectorModify()
{

}

BEGIN_MESSAGE_MAP(CPropertyGridVectorModify, CMFCPropertyGridCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



//This is an event when you click vector information.
void CPropertyGridVectorModify::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty *pProp;
	pProp = HitTest(point, &clickArea);
	if (clickArea == CMFCPropertyGridProperty::ClickArea::ClickExpandBox)
	{
		CMFCPropertyGridCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	if (clickArea == CMFCPropertyGridProperty::ClickArea::ClickValue)
	{
			return;
	}
	CMFCPropertyGridCtrl::OnLButtonDown(nFlags, point);
}

void CPropertyGridVectorModify::OnAddVector()
{
	CWnd* pParent = GetParent();

	if (pSelectedProperty)
	{
		pParent->PostMessage(WM_TRANSLATION_MESSAGE_TO_PARENT_ADD_VECTOR, (WPARAM)this->m_hWnd, (LPARAM)pSelectedProperty);
	}
}

void CPropertyGridVectorModify::OnDeleteVector()
{
	CWnd* pParent = GetParent();

	if (pSelectedProperty)
	{
		pParent->PostMessage(WM_TRANSLATION_MESSAGE_TO_PARENT_DELETE_VECTOR, (WPARAM)this->m_hWnd, (LPARAM)pSelectedProperty);
	}
}

void CPropertyGridVectorModify::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMFCPropertyGridProperty::ClickArea clickArea;
	CMFCPropertyGridProperty *pProp;
	pProp = HitTest(point, &clickArea);
	if (clickArea == CMFCPropertyGridProperty::ClickArea::ClickExpandBox)
	{
		CMFCPropertyGridCtrl::OnLButtonDblClk(nFlags, point);
		return;
	}

	if (pProp != NULL)
	{
		pSelectedProperty = pProp;

		CWnd* pParent = GetParent();
		pParent->PostMessage(WM_TRANSLATION_MESSAGE_TO_CURRENT_SELECTION_ADD, (WPARAM)this->m_hWnd, (LPARAM)pSelectedProperty);
	}
}