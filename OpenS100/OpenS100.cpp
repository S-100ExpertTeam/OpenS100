
// OpenS100.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
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
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#ifdef _DEBUG

#define new DEBUG_NEW

#undef THIS_FILE

static char THIS_FILE[] = __FILE__;

#endif


// COpenS100App
BEGIN_MESSAGE_MAP(COpenS100App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COpenS100App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()





// COpenS100App 생성
COpenS100App::COpenS100App() noexcept
{
	m_bHiColorIcons = TRUE;

	std::string str = "..\\TEMP"; // read path
	int mkFlag = _mkdir(str.c_str()); //If it's new, 0, if it exists or if it's not, -1.
	pView = nullptr;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OpenS100.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


COpenS100App::~COpenS100App()
{
}
// 유일한 COpenS100App 개체입니다.

COpenS100App theApp;


// COpenS100App 초기화

BOOL COpenS100App::InitInstance()
{


	// Gdiplus 초기화
	GdiplusStartupInput GdiplusStartupInput;
	if (::GdiplusStartup(&gdiplusToken, &GdiplusStartupInput, NULL) != Ok)
	{
		AfxMessageBox(_T("Error : Failed to initialize GDI+ library!"));
		return FALSE;
	}


	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	//도커블 팬 레지스트리 삭제
	CWinAppEx::CleanState();


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COpenS100Doc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(COpenS100View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	 
	
	//메인 윈도우 제목 변경
	m_pMainWnd->SetWindowTextW(L"OpenS100");

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	COpenS100View* pView = (COpenS100View*)pMain->GetActiveView();

	// 설정 파일 로딩
	LoadSettings();

	pView->OpenWorldMap();

	InitStartingLocation();
	
	return TRUE;
}

int COpenS100App::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// COpenS100App 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void COpenS100App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COpenS100App 사용자 지정 로드/저장 방법
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