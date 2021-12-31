// DOperator.cpp : 구현 파일입니다.
//
#include "pch.h"
#include "OpenS100.h"
#include "DockablePaneAttributeList.h"

IMPLEMENT_DYNAMIC(CDockablePaneAttributeList, CDockablePane)

CDockablePaneAttributeList::CDockablePaneAttributeList()
{

}

CDockablePaneAttributeList::~CDockablePaneAttributeList()
{

}

BEGIN_MESSAGE_MAP(CDockablePaneAttributeList, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDC_BT_INSERT, OnUpdateButton)
END_MESSAGE_MAP()

int CDockablePaneAttributeList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	CRect rectDummy;
	const DWORD dwtreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_treeCtrl.Create(dwtreeStyle, rectDummy, this, 2))
	{
		TRACE0("I couldn't make a tree.");
		return -1;
	
	}

	if (!m_button.Create(_T("INSERT"), WS_VISIBLE, rectDummy, this, IDC_BT_INSERT))
	{
		TRACE0("I couldn't make a button.\n");
		return -1;      
	}

	return 0;
}


void CDockablePaneAttributeList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(&rect, &CBrush(RGB(255, 255, 255)));
}


void CDockablePaneAttributeList::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AbjustLayout();
}

void CDockablePaneAttributeList::AbjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_treeCtrl.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.right, rectClient.bottom-25, SWP_NOACTIVATE | SWP_NOZORDER);
	m_button.SetWindowPos(NULL, rectClient.left, rectClient.bottom - 25, rectClient.right, 25, 0);
}

void CDockablePaneAttributeList::OnUpdateButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CDockablePaneAttributeList::RemoveAll()
{
	m_treeCtrl.DeleteAllItems();
}



void CDockablePaneAttributeList::SetShowTree(CGeoObject* ot)
{
	m_treeCtrl.SetShowTree(ot);
}

void CDockablePaneAttributeList::SetShowTree(FeatureCatalogue* fc, FeatureType* ft)
{
	m_treeCtrl.SetShowTree(fc, ft);
}

void CDockablePaneAttributeList::SetShowTree(FeatureCatalogue* fc, InformationType* ift)
{
	m_treeCtrl.SetShowTree(fc, ift); 
}

void CDockablePaneAttributeList::ComplexSetShowTree(FeatureCatalogue* fc, AttributeBinding* ab)
{
	m_treeCtrl.ComplexSetShowTree(fc, ab);
}

void CDockablePaneAttributeList::BaseSetShowThee()
{
	m_treeCtrl.BaseSetShowThee();
}
BOOL CDockablePaneAttributeList::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_BT_INSERT)
	{
		HTREEITEM item = m_treeCtrl.GetSelectedItem();   
		m_treeCtrl.InsertAttribute(item);
	}
	return CDockablePane::OnCommand(wParam, lParam);
}

void CDockablePaneAttributeList::SetParentProperty(CMFCPropertyGridProperty *pParentAttribute)
{
	m_treeCtrl.pParentAttribute = pParentAttribute;
	
}

void CDockablePaneAttributeList::SetindexNum(int indexNum)
{
	m_treeCtrl.selectIndexNum = indexNum;
}