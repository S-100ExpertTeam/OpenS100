// Configuration_1.cpp : implementation file
//
#include "pch.h"
#include "OpenS100.h"
#include "Configuration_1.h"
#include "afxdialogex.h"
#include "ConfigrationDlg.h"

#include "../GISLibrary/LayerManager.h"

IMPLEMENT_DYNAMIC(CConfiguration_1, CDialogEx)

BOOL CALLBACK EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID aFontCount)
{
	int far * aiFontCount = (int far *) aFontCount;

	// Record the number of raster, TrueType, and vector  
	// fonts in the font-count array.  

	if (FontType & RASTER_FONTTYPE)
		aiFontCount[0]++;
	else if (FontType & TRUETYPE_FONTTYPE)
		aiFontCount[2]++;
	else
		aiFontCount[1]++;

	if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2])
		return TRUE;
	else
		return FALSE;

	UNREFERENCED_PARAMETER(lplf);
	UNREFERENCED_PARAMETER(lpntm);
}

CConfiguration_1::CConfiguration_1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfiguration_1::IDD, pParent)
{
}

CConfiguration_1::~CConfiguration_1()
{
}

void CConfiguration_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TWO_SHADE, m_twoShade);
	DDX_Control(pDX, IDC_USE_NOBJNM, m_useNationalObjectName);
	DDX_Control(pDX, IDC_CHECK_IGNORE_SCALE_MIN, checkBoxIgnoreScaleMin);
	DDX_Control(pDX, IDC_CHECK_SHALLOW_PATTERN, checkBoxShallowPattern);
	DDX_Control(pDX, IDC_CHECK_SIMPLIFY_POINT, checkBoxSimplifyPoint);
	DDX_Control(pDX, IDC_CHECK_SHOW_ISOLATED_DANGER, checkBoxShowIsolatedDanger);
	DDX_Control(pDX, IDC_CHECK_SIMPLIFY_LINE, checkBoxSimplifyLine);
	DDX_Control(pDX, IDC_CHECK_FULL_LIGHT_SECTOR, checkBoxFullSector);

	DDX_Control(pDX, IDC_SAFETY_DEPTH, m_safetyDepth);
	DDX_Control(pDX, IDC_SAFETY_WATER_LEVEL, m_safetyWaterLevel);
	DDX_Control(pDX, IDC_VERY_SHALLOW_WATER, m_veryShallowWaterLevel);
	DDX_Control(pDX, IDC_VERY_DEEP_WATER, m_veryDeepWaterLevel);
	DDX_Radio(pDX, IDC_COLOR_DB, m_colorScheme);
	DDX_Radio(pDX, IDC_DPM_BASE, m_displayMode);

	DDX_Control(pDX, IDC_CHECK_SHOW_INFORM01, showinform01);
}


BEGIN_MESSAGE_MAP(CConfiguration_1, CDialogEx)
	ON_WM_CLOSE()

END_MESSAGE_MAP()

BOOL CConfiguration_1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	checkBoxIgnoreScaleMin.SetCheck(!ENCCommon::APPLY_SCALE_MIN);
	checkBoxFullSector.SetCheck(ENCCommon::FULL_SECTORS);

	m_safetyWaterLevel.SetWindowText(_bstr_t(ENCCommon::SAFETY_CONTOUR));
	m_veryShallowWaterLevel.SetWindowText(_bstr_t(ENCCommon::SHALLOW_CONTOUR));
	m_veryDeepWaterLevel.SetWindowText(_bstr_t(ENCCommon::DEEP_CONTOUR));

	m_colorScheme = (int)ENCCommon::m_eColorTable;
	m_displayMode = (int)ENCCommon::DISPLAY_MODE;

	showinform01.SetCheck(ENCCommon::Show_INFORM01);

	UpdateData(FALSE);

	return TRUE;
}

void CConfiguration_1::OnClose()
{
	CDialogEx::OnClose();
}

void CConfiguration_1::OnOK()
{

}

void CConfiguration_1::OnCancel()
{

}

void CConfiguration_1::SetFontList(std::vector<CString> fontList)
{
	m_fontList.clear();

	m_fontList.insert(m_fontList.begin(), fontList.begin(), fontList.end());
}

std::vector<CString>* CConfiguration_1::GetFontList()
{
	return &m_fontList;
}