//
#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "OpenS100.h"
#include "MainFrm.h"
#include "OpenS100Doc.h"
#include "OpenS100View.h"

#include "..\\GeoMetryLibrary\\ENCCommon.h"
#include "..\\GISLibrary\\GISLibrary.h"
#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"


#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <direct.h>
#include <fstream>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

#ifdef _DEBUG

#define new DEBUG_NEW

#undef THIS_FILE

static char THIS_FILE[] = __FILE__;

#endif


BEGIN_MESSAGE_MAP(COpenS100App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COpenS100App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


COpenS100App::COpenS100App() noexcept
{
	m_bHiColorIcons = TRUE;

	std::string str = "..\\TEMP"; // read path
	int mkFlag = _mkdir(str.c_str()); //If it's new, 0, if it exists or if it's not, -1.
	pView = nullptr;

	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	SetAppID(_T("OpenS100.AppID.NoVersion"));

}


COpenS100App::~COpenS100App()
{
}

COpenS100App theApp;


BOOL COpenS100App::InitInstance()
{


	GdiplusStartupInput GdiplusStartupInput;
	if (::GdiplusStartup(&gdiplusToken, &GdiplusStartupInput, NULL) != Ok)
	{
		AfxMessageBox(_T("Error : Failed to initialize GDI+ library!"));
		return FALSE;
	}


	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("create by local application"));
	LoadStdProfileSettings(4);  


	CWinAppEx::CleanState();


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COpenS100Doc),
		RUNTIME_CLASS(CMainFrame),       
		RUNTIME_CLASS(COpenS100View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	 
	
	m_pMainWnd->SetWindowTextW(L"OpenS100");

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	COpenS100View* pView = (COpenS100View*)pMain->GetActiveView();

	LoadSettings();

	pView->OpenWorldMap();

	InitStartingLocation();
	
	return TRUE;
}

int COpenS100App::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void COpenS100App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void COpenS100App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void COpenS100App::LoadCustomState()
{
}

void COpenS100App::SaveCustomState()
{
}

void COpenS100App::LoadSettings()
{
	ENCCommon::Open(L"..\\ProgramData\\data\\settings.txt");
	gisLib->ChangeDisplayFont();
	
	return;
}

void COpenS100App::InitStartingLocation()
{
	CRect rect;
	pView->GetClientRect(rect);

	CString strScale = L"10000000";
	CString strMox = L"126.5";
	CString strMoy = L"36.5";

	CStdioFile file;
	if (file.Open(_T("..\\ProgramData\\data\\init.txt"), CFile::modeRead))
	{
		file.ReadString(strScale);
		file.ReadString(strMox);
		file.ReadString(strMoy);
		file.Close();
	}

	double scale = _wtof(strScale);
	if (scale < 1000)
	{
		scale = 1000;
	}

	double mox = _wtof(strMox);
	double moy = _wtof(strMoy);

	gisLib->SetScale((int)scale);
	projection(mox, moy);
	gisLib->MoveMap(rect.Width() / 2, rect.Height() / 2, mox, moy);
	gisLib->AdjustScreenMap();
}

void COpenS100App::MapRefresh()
{
	if (pView)
	{
		pView->MapRefresh();
	}
}

// Save txt whenever the setting changes.
void COpenS100App::SaveSettings()
{
	ENCCommon::Save(L"..\\ProgramData\\data\\settings.txt");
	return;
}