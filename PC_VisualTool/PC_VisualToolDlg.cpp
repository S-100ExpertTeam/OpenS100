
// PC_VisualToolDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PC_VisualTool.h"
#include "PC_VisualToolDlg.h"
#include "afxdialogex.h"
#include <string>
#include <filesystem>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPCVisualToolDlg dialog

CPCVisualToolDlg::CPCVisualToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PC_VISUALTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPCVisualToolDlg::~CPCVisualToolDlg()
{
	
}

void CPCVisualToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
}

BEGIN_MESSAGE_MAP(CPCVisualToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPCVisualToolDlg message handlers

BOOL CPCVisualToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPCVisualToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPCVisualToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPCVisualToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPCVisualToolDlg::OnDropFiles(HDROP hDropInfo)
{
	CString sFileList(_T("")); 	
	CString sFolderName = _T("");
	TCHAR szFileName[MAX_PATH] = {0, }; 
	int iCount = ::DragQueryFile( hDropInfo, 0xFFFFFFFF, szFileName, sizeof(szFileName) ); 

	memset(szFileName, 0, sizeof(szFileName));
	::DragQueryFile( hDropInfo, 0, szFileName, sizeof(szFileName) );
	CString sFileName; 
	sFileName.Format( _T("%s"), szFileName );
	sFileList += sFileName; 

	sFolderName = sFileList.Left(sFileList.ReverseFind('\\') + 1);
	
	
	SetDlgItemText(IDC_EDIT1, sFileName);

	XMLreadFile(sFolderName);
	::DragFinish( hDropInfo );

	CDialogEx::OnDropFiles(hDropInfo);
}

void CPCVisualToolDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_TreeCtrl.DeleteAllItems();

	if (m_PC != nullptr)
		delete m_PC;
}


void CPCVisualToolDlg::XMLreadFile(CString filepath)
{
	//미리 들어있던 데이터 지워주기
	if(m_PC != nullptr)
		m_PC->Delete();

	//filepath
	std::wstring path= CStringW(filepath);
	m_PC = new PortrayalCatalogue(L"..\\ProgramData\\S101_Portrayal\\",true);

	SetXmlTreeControl(m_PC);
}


void CPCVisualToolDlg::SetTreeCtrlColorProfile(std::vector< S100_ColorProfile*>* vecColorProfiles)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("ColorPeofiles"), NULL, NULL);

	for (auto value : *vecColorProfiles)
	{
		//color profile id
		temp.Format(_T("colorProfile  id = %s"), value->Getid().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name
		temp.Format(_T("name = %s"), value->Getdescription().Getname().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);
		//description - description
		temp.Format(_T("description = %s"), value->Getdescription().Getdescription().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);
		//description - language
		temp.Format(_T("language = %s"), value->Getdescription().Getlanguage().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);

		//filename
		temp.Format(_T("fileName = %s"), value->GetfileName().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//filetype
		temp.Format(_T("fileType = %s"), value->GetfileType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//fileFormat
		temp.Format(_T("fileFormat = %s"), value->GetfileFormat().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
	}
}


void CPCVisualToolDlg::SetTreeCtrlSymbols(std::vector<ExternalFile*>* vecSymbols)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("symbols"), NULL, NULL);

	for (auto value : *vecSymbols)
	{
		//color profile id
		temp.Format(_T("symbol  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto description : *vecDescription)
		{
			temp.Format(_T("name = %s"), description ->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), description->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), description->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}

		//filename
		temp.Format(_T("fileName = %s"), value->GetFileName().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//filetype
		temp.Format(_T("fileType = %s"), value->GetFileType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//fileFormat
		temp.Format(_T("fileFormat = %s"), value->GetFileFormat().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		
	}
}


void CPCVisualToolDlg::SetTreeLinStyles(std::vector< ExternalFile*>* vecLinStyles)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;
	
	h_root = m_TreeCtrl.InsertItem(_T("lineStyles"), NULL, NULL);

	for (auto value : *vecLinStyles)
	{
		//color profile id
		temp.Format(_T("lineStyle  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto description : *vecDescription)
		{
			temp.Format(_T("name = %s"), description->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), description->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), description->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}

		//filename
		temp.Format(_T("fileName = %s"), value->GetFileName().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//filetype
		temp.Format(_T("fileType = %s"), value->GetFileType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//fileFormat
		temp.Format(_T("fileFormat = %s"), value->GetFileFormat().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
	}
}


void CPCVisualToolDlg::SetTreeAreaFills(std::vector< ExternalFile*>* vecAreaFills)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;
	
	h_root = m_TreeCtrl.InsertItem(_T("areaFills"), NULL, NULL);

	for (auto value : *vecAreaFills)
	{
		//color profile id
		temp.Format(_T("areaFill  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto description : *vecDescription)
		{
			temp.Format(_T("name = %s"), description->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), description->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), description->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}

		//filename
		temp.Format(_T("fileName = %s"), value->GetFileName().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//filetype
		temp.Format(_T("fileType = %s"), value->GetFileType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		//fileFormat
		temp.Format(_T("fileFormat = %s"), value->GetFileFormat().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
	}
}


void CPCVisualToolDlg::SetTreeViewingGroups(std::vector<ViewingGroup*>* vecViewingGroups)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("viewingGroups"), NULL, NULL);
	
	for (auto value : *vecViewingGroups)
	{
		//color profile id
		temp.Format(_T("viewingGroup  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto description : *vecDescription)
		{
			temp.Format(_T("name = %s"), description->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), description->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - languages
			temp.Format(_T("language = %s"), description->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}
	}
}


void CPCVisualToolDlg::SetTreeFoundationMode(std::vector<ViewingGroup*>* vecFoundationmode)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id;

	h_root = m_TreeCtrl.InsertItem(_T("foundationMode"), NULL, NULL);

	for (auto value : *vecFoundationmode)
	{
		//color profile id
		temp.Format(_T("viewingGroup = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);
	}
}


void CPCVisualToolDlg::SetTreeViewingGroupLayers(std::vector<ViewingGroupLayer*>* vecViewingGroupLayers)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("viewingGroupLayers"), NULL, NULL);

	for (auto value : *vecViewingGroupLayers)
	{
		//color profile id
		temp.Format(_T("viewingGroupLayer  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto descriptionVal : *vecDescription)
		{
			//description - name
			temp.Format(_T("name = %s"), descriptionVal->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), descriptionVal->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), descriptionVal->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}


		std::vector < ViewingGroup* >* vecViewingGroup = value->GetViewingGroupVector();
		for (auto viewingGroup : *vecViewingGroup)
		{
			temp.Format(_T("viewingGroup = %d"), viewingGroup->value);
			m_TreeCtrl.InsertItem(temp, h_id, NULL);
		}
	}
}


void CPCVisualToolDlg::SetTreeDisplayModes(std::vector<DisplayMode*>* vecDisplayModes)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("displayModes"), NULL, NULL);

	for (auto value : *vecDisplayModes)
	{
		//color profile id
		temp.Format(_T("displayMode  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto descriptionVal : *vecDescription)
		{
			//description - name
			temp.Format(_T("name = %s"), descriptionVal->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), descriptionVal->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), descriptionVal->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}

		std::vector < ViewingGroupLayer* >* vecViewingGroupLayer = value->GetViewingGroupLayerVector();
		for (auto viewingGroupLayer : *vecViewingGroupLayer)
		{
			temp.Format(_T("viewingGroupLayer = %s"), viewingGroupLayer->GetValue().c_str());
			m_TreeCtrl.InsertItem(temp, h_id, NULL);
		}
	}
}


void CPCVisualToolDlg::SetTreeDisplayPlanes(std::vector< DisplayPlane*>* vecDisplayPlanes)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("displayPlanes"), NULL, NULL);

	for (auto value : *vecDisplayPlanes)
	{
		//color profile id
		temp.Format(_T("displayPlane  id = %s   order = %d"), value->GetId().c_str(), value->GetOrder());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto descriptionVal : *vecDescription)
		{
			//description - name
			temp.Format(_T("name = %s"), descriptionVal->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), descriptionVal->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), descriptionVal->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}
	}
}


void CPCVisualToolDlg::SetTreeContext(std::vector<ContextParameter*>* vecContext)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("context"), NULL, NULL);

	for (auto value : *vecContext)
	{
		//color profile id
		temp.Format(_T("parameter  id = %s"), value->GetId().c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);

		std::vector<S100_Description*>* vecDescription = value->GetDescription();
		for (auto descriptionVal : *vecDescription)
		{
			//description - name
			temp.Format(_T("name = %s"), descriptionVal->Getname().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - description
			temp.Format(_T("description = %s"), descriptionVal->Getdescription().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
			//description - language
			temp.Format(_T("language = %s"), descriptionVal->Getlanguage().c_str());
			m_TreeCtrl.InsertItem(temp, h_description, NULL);
		}

		temp.Format(_T("type = %s"), ParameterTypeToString(*value->GetType()).c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);

		temp.Format(_T("default = %s"), value->GetDefault().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
	}
}


void CPCVisualToolDlg::SetTreeRuleFiles(std::vector<S100_RuleFile*>* vecRuleFiles)
{
	CString temp;

	HTREEITEM h_root;
	HTREEITEM h_id, h_description;

	h_root = m_TreeCtrl.InsertItem(_T("rules"), NULL, NULL);

	for (auto value : *vecRuleFiles)
	{
		//color profile id
		temp.Format(_T("ruleFile   id = %s"), pugi::as_wide(value->GetID()).c_str());
		h_id = m_TreeCtrl.InsertItem(temp, h_root, NULL);

		//description
		h_description = m_TreeCtrl.InsertItem(_T("description"), h_id, NULL);
		//description - name
		temp.Format(_T("name = %s"), value->GetDescription()->Getname().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);
		//description - description
		temp.Format(_T("description = %s"), value->GetDescription()->Getdescription().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);
		//description - language
		temp.Format(_T("language = %s"), value->GetDescription()->Getlanguage().c_str());
		m_TreeCtrl.InsertItem(temp, h_description, NULL);


		temp.Format(_T("fileName = %s"), value->GetFileName().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		temp.Format(_T("fileType = %s"), value->GetFileType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		temp.Format(_T("fileFormat = %s"), value->GetFileFormat().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
		temp.Format(_T("ruleType = %s"), value->GetRuleType().c_str());
		m_TreeCtrl.InsertItem(temp, h_id, NULL);
	}
}


std::wstring CPCVisualToolDlg::ParameterTypeToString(ParameterType val)
{
	switch (val)
	{
	case Portrayal::Boolean:
		return _T("Boolean");
	case Portrayal::Integer:
		return _T("Integer");
	case Portrayal::Double:
		return _T("Double");
	case Portrayal::String:
		return _T("String");
	case Portrayal::Date:
		return _T("Date");
	default:
		break;
	}
	return _T("");
}


void CPCVisualToolDlg::SetXmlTreeControl(PortrayalCatalogue* m_PC)
{
	m_TreeCtrl.DeleteAllItems();

	S100_ColorProfiles* colorProfiles =  m_PC->GetColorProfiles();
	std::vector< S100_ColorProfile*>* vecColorProfiles = colorProfiles->GetColorProfilesVector();
	SetTreeCtrlColorProfile(vecColorProfiles);

	S100_Symbols* symbols = m_PC->GetSymbols();
	std::vector<ExternalFile*>* vecSymbols = symbols->GetSymbolsVector();
	SetTreeCtrlSymbols(vecSymbols);

	S100_LineStyles* LinStyles = m_PC->GetTreeLineStyles();
	std::vector< ExternalFile*>* vecLinStyles = LinStyles->GetLineStyleFilesVector();
	SetTreeLinStyles(vecLinStyles);

	S100_AreaFills*  areaFills = m_PC->GetAreaFills();
	std::vector< ExternalFile*> * vecAreaFills= areaFills->GetAreaFillFilesVecter();
	SetTreeAreaFills(vecAreaFills);
	
	 ViewingGroups* viewingGroups = m_PC->GetViewingGroups();
	std::vector<ViewingGroup*>* vecViewingGroups = viewingGroups->GetViewingGroup();
	SetTreeViewingGroups(vecViewingGroups);
	
	FoundationMode* foundationmode = m_PC->GetFoundationMode();
	std::vector<ViewingGroup*>* vecFoundationmode = foundationmode->GetViewingGroupVector();
	SetTreeFoundationMode(vecFoundationmode);

	ViewingGroupLayers* viewingGroupLayers = m_PC->GetViewingGroupLayers();
	std::vector<ViewingGroupLayer*>* vecViewingGroupLayers = viewingGroupLayers->GetViewingGroupLayerVector();
	SetTreeViewingGroupLayers(vecViewingGroupLayers);
	
	DisplayModes* displayModes = m_PC->GetDisplaModes();
	std::vector<DisplayMode*>*  vecDisplayModes = displayModes->GetDisplayModeVector();
	SetTreeDisplayModes(vecDisplayModes);

	DisplayPlanes* displayPlanes = m_PC->GetDisplayPlanes();
	std::vector< DisplayPlane*>* vecDisplayPlanes = displayPlanes->GetDisplayPlanesVector();
	SetTreeDisplayPlanes(vecDisplayPlanes);

	Context*  context = m_PC->GetContext();
	std::vector<ContextParameter*>* vecContext = context->GetContextParameter();
	SetTreeContext(vecContext);

	S100_Rules* rules = m_PC->GetRules();
	std::vector<S100_RuleFile*>* vecRuleFiles = rules->GetRuleFilesVector();
	SetTreeRuleFiles(vecRuleFiles);

}

