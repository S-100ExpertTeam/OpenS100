#include "pch.h"
#include "resource.h"
#include "OpenS100.h"
#include "OpenS100View.h"
#include "DialogTreeAttributeList.h"
#include "DialogDockCurrentSelection.h"

#include "..\\GeoMetryLibrary\\ENCGeometry.h"
#include "..\\FeatureCatalog\\FeatureCatalogue.h"
#include "..\\GISLibrary\\NewFeatureManager.h"

IMPLEMENT_DYNAMIC(CDialogTreeAttributeList, CTreeCtrlExt)

CDialogTreeAttributeList::CDialogTreeAttributeList()
{

}

CDialogTreeAttributeList::~CDialogTreeAttributeList()
{

}

BEGIN_MESSAGE_MAP(CDialogTreeAttributeList, CTreeCtrlExt)
	ON_NOTIFY_REFLECT(NM_CLICK, &CDialogTreeAttributeList::OnNMClick)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CDialogTreeAttributeList::OnNMDblclk)
END_MESSAGE_MAP()

void CDialogTreeAttributeList::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint p;
	GetCursorPos(&p);
	::ScreenToClient(m_hWnd, &p);
	UINT nFlags = 0;
	HTREEITEM hltem = NULL;
	hltem = HitTest(p, &nFlags);
	HTREEITEM start = GetRootItem();

	while (GetItemText(start).Compare(_T("")) != 0)
	{
		SetItemColor(start, RGB(0, 0, 0));

		start = GetNextSiblingItem(start);
	}

	HTREEITEM start1 = GetRootItem();

	while (GetItemText(start1).Compare(_T("")) != 0)
	{
		SetItemColor(start1, RGB(0, 0, 0));

		start1 = GetNextSiblingItem(start1);
	}

	if (hltem)
	{
		SelectItem(hltem);
		SetFocus();

		while (GetItemText(start).Compare(_T("")) != 0)
		{
			SetCheck(start, FALSE);
			start = GetNextSiblingItem(start);
		}

		SetCheck(hltem, TRUE);

		selectIndex = (int)GetItemData(hltem);
	}

	*pResult = 0;
}

int CDialogTreeAttributeList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBitmap Bmp1;
	Bmp1.LoadBitmap(IDB_BITMAP3);
	ImgList.Create(16, 16, ILC_COLOR24, 3, 0);
	ImgList.Add(&Bmp1, RGB(0, 233, 233));

	SetImageList(&ImgList, TVSIL_NORMAL);
	return 0;
}

void CDialogTreeAttributeList::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM item = GetSelectedItem();
	InsertAttribute(item);

	*pResult = 0;
}

void CDialogTreeAttributeList::SetShowTree(CGeoObject* ot)
{
	m_type = 1;

	DeleteAllItems();

	if (ot == NULL)
	{
		return;
	}

}

void CDialogTreeAttributeList::SetShowTree(FeatureCatalogue* _fc, FeatureType* ft)
{
	{
		return;
	}
}

void CDialogTreeAttributeList::SetShowTree(FeatureCatalogue* _fc, InformationType* ift)
{
	m_type = 2;

		return;
}

void CDialogTreeAttributeList::InsertAttribute(HTREEITEM &item)
{
	if (item)
	{
		if (m_type == 2)
		{
			AttributeBinding* ab = (AttributeBinding*)GetItemData(item);
			theApp.m_DockablePaneEditWindow.AddAttribute(ab, pParentAttribute, selectIndexNum);
		}
	}
}

void CDialogTreeAttributeList::ComplexSetShowTree(FeatureCatalogue* _fc, AttributeBinding* ab)
{
	std::wstring name;
	auto attribute = ab->GetAttribute().Getattributes();
	for (auto i = attribute.begin(); i != attribute.end(); i++)
	{
		if (i->Getname().compare(L"ref") == 0)
		{
			name = i->Getvalue();
		}
	}

	auto fc = _fc;
	if (nullptr == fc)
	{
		return;
	}

	auto fc_simpleAttribute = fc->GetSimpleAttribute(name);
	auto fc_complexAttribute = fc->GetSimpleAttribute(name);

	TVINSERTSTRUCT tvi;
	tvi.hParent = TVI_ROOT;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;

	if (fc_simpleAttribute)
	{
		tvi.item.pszText = LPWSTR(fc_simpleAttribute->GetName().c_str());
		tvi.item.lParam = (LPARAM)fc_simpleAttribute;
	}
	else if (fc_complexAttribute)
	{
		tvi.item.pszText = LPWSTR(fc_complexAttribute->GetName().c_str());
		tvi.item.lParam = (LPARAM)fc_complexAttribute;
	}
	else
	{
		tvi.item.pszText = LPWSTR(name.c_str());
	}
	HTREEITEM insertedItem = InsertItem(&tvi);
	SetItemData(insertedItem, (DWORD_PTR)ab);

}

void CDialogTreeAttributeList::BaseSetShowThee() //Create the most basic structure selected.
{
	
	auto featureType = theApp.pView->m_pNewFeatureManager->GetFeatureType();
	if (featureType!=nullptr)
	{
		// If simple is selected from the attribute, this part of the data is strange.
		SetShowTree(theApp.pView->m_pNewFeatureManager->GetFeatureCatalog(), featureType);
	}

	auto informationType=theApp.m_DockablePaneCurrentSelection.pDlg->m_selectedInformationType;
	if (informationType!=nullptr)
	{
		SetShowTree(theApp.pView->m_pNewFeatureManager->GetFeatureCatalog(), informationType);
	}
}