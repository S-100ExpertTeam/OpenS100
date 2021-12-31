#include "pch.h"
#include "MFCTabCtrlMsgToParent.h"
#include "..\\OpenS100\\OpenS100.h"

CMFCTabCtrlMsgToParent::CMFCTabCtrlMsgToParent()
{

}

CMFCTabCtrlMsgToParent::~CMFCTabCtrlMsgToParent()
{

}

BEGIN_MESSAGE_MAP(CMFCTabCtrlMsgToParent, CMFCTabCtrl)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

LRESULT CMFCTabCtrlMsgToParent::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lparam;
	if (!pProp)
		return 0;

	GetParent()->SendMessage(AFX_WM_PROPERTY_CHANGED, wparam, lparam);

	return 0;
}

BOOL CMFCTabCtrlMsgToParent::PreTranslateMessage(MSG* pMsg)
{
	return CMFCTabCtrl::PreTranslateMessage(pMsg);
}


void CMFCTabCtrlMsgToParent::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCTabCtrl::OnLButtonDown(nFlags, point);
	int TabNum = GetActiveTab();

	if (TabNum == 0)
	{
		theApp.m_DockablePaneEditWindow.SetAttributes();
	}
	else if (TabNum == 1)
	{
		theApp.m_DockablePaneEditWindow.SetVectors();
	}
}