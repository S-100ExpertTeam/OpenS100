#include "pch.h"
#include "OpenS100.h"
#include "Configuration_4.h"
#include "afxdialogex.h"
#include "ConfigrationDlg.h"
#include "DialogDockLayerManager.h"

#include "../GISLibrary/LayerManager.h"
#include "../GISLibrary/GISLibrary.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

// Configuration_4 dialog box
IMPLEMENT_DYNAMIC(CConfiguration_4, CDialogEx)

CConfiguration_4::CConfiguration_4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CConfiguration_4::IDD, pParent)
{

}


CConfiguration_4::~CConfiguration_4()
{

}

void CConfiguration_4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEWINGGROUP, m_viewingGroupList);
}

BEGIN_MESSAGE_MAP(CConfiguration_4, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfiguration_4::OnBnClickedButton_initialization)
END_MESSAGE_MAP()

BOOL CConfiguration_4::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	try
	{
		CRect rect;

		m_viewingGroupList.GetClientRect(rect);
		m_viewingGroupList.InsertColumn(0, _T("on/off"), LVCFMT_LEFT, 60);
		m_viewingGroupList.InsertColumn(1, _T("name"), LVCFMT_LEFT, 100);
		m_viewingGroupList.InsertColumn(2, _T("language"), LVCFMT_LEFT, 40);
		m_viewingGroupList.InsertColumn(3, _T("description"), LVCFMT_LEFT, rect.Width() - 200);

		m_viewingGroupList.SetExtendedStyle(m_viewingGroupList.GetExtendedStyle() | LVS_EX_CHECKBOXES);

		if (theApp.m_pDockablePaneLayerManager.pDlg == nullptr) //If you don't have the opened price,
		{
			return false;
		}

		auto pc = gisLib->GetPC();
		if (pc == nullptr)
		{
			return false;
		}

		CString cs(_T(""));
		int i = 1;
		for (auto viewing : *pc->GetViewingGroups()->GetViewingGroup())
		{
			for (S100_Description* dis : *viewing->GetDescription())
			{
				std::wstring Name = dis->Getname();
				std::wstring Lag = dis->Getlanguage();
				std::wstring Des = dis->Getdescription();

				int nItem = m_viewingGroupList.InsertItem(i, cs);
				m_viewingGroupList.SetItemText(nItem, 1, Name.c_str());
				m_viewingGroupList.SetItemText(nItem, 2, Lag.c_str());
				m_viewingGroupList.SetItemText(nItem, 3, Des.c_str());

				viewing_map[Name] = dis;
				i++;
			}
		}
		//I check all the details.
		int nCount = m_viewingGroupList.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			m_viewingGroupList.SetCheck(i);
		}
		return true;  // return TRUE unless you set the focus to a control
	}
	catch (int exceptionCode)
	{
		exceptionCode = 0;
		return false;
	}
}

// In Setting, the value of turning off the viewing group comes in.
void CConfiguration_4::OnBnClickedButton_initialization()
{
	int nCount = m_viewingGroupList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		auto click = m_viewingGroupList.GetCheck(i);
		if (click == false)
		{
			CString name = m_viewingGroupList.GetItemText(i, 1);
			auto isView=viewing_map.find(std::wstring(name));
			if (isView != viewing_map.end())
			{
				S100_Description* viewing = viewing_map[std::wstring(name)];
				viewing->SetOn(false);
			
			}
			CString str;
			str.Format(_T("%s is visible false \n"), name);
			OutputDebugString(str);
		}
	}
	return;
}


BOOL CConfiguration_4::PreTranslateMessage(MSG* pMsg) // blocked the ESC button.
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		pMsg->wParam = NULL;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}