#include "pch.h"
#include "ConfigrationDlg.h"
#include "OpenS100.h"
#include "OpenS100View.h"
#include "DialogDockLayerManager.h"

#include "../LibMFCUtil/StringTokenizer.h"
#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/GISLibrary.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include <fstream>

IMPLEMENT_DYNAMIC(CConfigrationDlg, CDialogEx)

CConfigrationDlg::CConfigrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigrationDlg::IDD, pParent)
{
	m_config.APPLY_SCALE_MIN = ENCCommon::APPLY_SCALE_MIN;
	m_config.DEEP_CONTOUR = ENCCommon::DEEP_CONTOUR;
	m_config.DISPLAY_MODE = ENCCommon::DISPLAY_MODE;
	m_config.DrawingType = ENCCommon::DrawingType;
	m_config.OVER_GROUP = ENCCommon::OVER_GROUP;
	m_config.SAFETY_CONTOUR = ENCCommon::SAFETY_CONTOUR;
	m_config.SHALLOW_CONTOUR = ENCCommon::SHALLOW_CONTOUR;
	m_config.FULL_SECTORS = ENCCommon::FULL_SECTORS;
	m_config.TEXTOUT = ENCCommon::TEXTOUT;
	m_config.m_eColorTable = ENCCommon::m_eColorTable;

	m_config.DISPLAY_FONT_NAME = ENCCommon::DISPLAY_FONT_NAME;
	m_config.DISPLAY_FONT_SIZE = ENCCommon::DISPLAY_FONT_SIZE;

	//arrangement enum 
	m_config.objectDisplaySettings.insert(ENCCommon::objectDisplaySettings.begin(), ENCCommon::objectDisplaySettings.end());
	bObjectDisplaySettingChanges = false;
}

CConfigrationDlg::~CConfigrationDlg()
{

}

void CConfigrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIG_TAB, m_tab);
}


BEGIN_MESSAGE_MAP(CConfigrationDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONFIG_TAB, &CConfigrationDlg::OnTcnSelchangeConfigTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CONFIG_TAB, &CConfigrationDlg::OnTcnSelchangingConfigTab)
	ON_BN_CLICKED(IDOK, &CConfigrationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConfigrationDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_APPLY, &CConfigrationDlg::OnBnClickedApply)
END_MESSAGE_MAP()


// CConfigrationDlg message handlers


BOOL CConfigrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString    strOne = _T("Context Parameter");
	CString    strS101 = _T("Features");

	m_tab.InsertItem(1, strOne);
	m_tab.InsertItem(2, strS101);

	CRect r;
	m_tab.GetWindowRect(&r);

	// Main
	mainTab.Create(IDD_DIALOG_CONFIG_1, &m_tab);
	mainTab.ShowWindow(SW_SHOW);
	mainTab.GetWindowRect(&r);
	mainTab.MoveWindow(5, 25, r.Width() - 10, r.Height() - 30);

	//// S-101
	s101Tab.Create(IDD_DIALOG_CONFIG_4, &m_tab);
	s101Tab.ShowWindow(SW_HIDE);
	s101Tab.MoveWindow(5, 25, r.Width() - 10, r.Height() - 30);
	s101Tab.m_pParent = this;

	m_tab.SetCurSel(0);

	mainTab.m_pParent = this;

	return true;
}


void CConfigrationDlg::OnTcnSelchangeConfigTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_tab.GetCurSel(); // After the change, the selected tap number

	switch (nTab)
	{
	case 0:
		mainTab.ShowWindow(SW_SHOW);
		break;
	case 1:
		s101Tab.ShowWindow(SW_SHOW);
		break;

	}
	*pResult = 0;
}


void CConfigrationDlg::OnTcnSelchangingConfigTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_tab.GetCurSel(); // Tab number selected before it changes

	switch (nTab)
	{
	case 0:
		mainTab.ShowWindow(SW_HIDE);
		break;
	case 1:
		s101Tab.ShowWindow(SW_HIDE);
		break;

	}
	*pResult = 0;
}

void CConfigrationDlg::OnBnClickedOk()
{
	OnBnClickedApply();
	CDialogEx::OnOK();
}

void CConfigrationDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CConfigrationDlg::OnClose()
{
	CDialogEx::OnClose();
}

void CConfigrationDlg::InitS101FeatureTypes(FeatureCatalogue* fc)
{
	m_config.featureDisplaySettings.clear();

	if (nullptr == fc)
	{
		return;
	}

	for (auto fti = fc->GetFeatureTypes()->GetFeatureType().begin(); fti != fc->GetFeatureTypes()->GetFeatureType().end(); fti++)
	{
		auto ft = fti->second;

		m_config.featureDisplaySettings.insert({ ft->GetName(), true });
	}
}

void CConfigrationDlg::InitControlValues()
{
	mainTab.m_safetyWaterLevel.SetWindowText(_bstr_t(m_config.SAFETY_CONTOUR));
	mainTab.m_veryShallowWaterLevel.SetWindowText(_bstr_t(m_config.SHALLOW_CONTOUR));
	mainTab.m_veryDeepWaterLevel.SetWindowText(_bstr_t(m_config.DEEP_CONTOUR));

	mainTab.m_colorScheme = (int)m_config.m_eColorTable;
	mainTab.m_displayMode = (int)m_config.DISPLAY_MODE;

	std::unordered_map<std::wstring, bool>::iterator fdsitor;

	mainTab.UpdateData(FALSE);
	s101Tab.UpdateData(FALSE);
}

void CConfigrationDlg::OnBnClickedApply()
{
	bool bMapRefresh = false;
	bool bChangeDisplayMode = false;
	bool bRebuildPortrayal = false;

	mainTab.UpdateData();
	s101Tab.UpdateData();

	CString value;

	m_config.APPLY_SCALE_MIN = mainTab.checkBoxIgnoreScaleMin.GetCheck() ? false : true;
	m_config.SIMPLIFIED_POINT_SYMBOL = mainTab.checkBoxSimplifyPoint.GetCheck() ? true : false;
	m_config.FULL_SECTORS = mainTab.checkBoxFullSector.GetCheck() ? true : false;

	mainTab.m_safetyWaterLevel.GetWindowText(value);
	m_config.SAFETY_CONTOUR = _ttoi(value);

	mainTab.m_veryShallowWaterLevel.GetWindowText(value);
	m_config.SHALLOW_CONTOUR = _ttoi(value);

	mainTab.m_veryDeepWaterLevel.GetWindowText(value);
	m_config.DEEP_CONTOUR = _ttof(value);

	m_config.DISPLAY_FONT_NAME = value;

	m_config.DISPLAY_FONT_SIZE = _ttoi(value);


	ASSERT(!(mainTab.m_colorScheme < (int)GeoMetryLibrary::ColorTable::Day || mainTab.m_colorScheme >(int)GeoMetryLibrary::ColorTable::Night));

	m_config.m_eColorTable = static_cast<GeoMetryLibrary::ColorTable>(mainTab.m_colorScheme);
	ASSERT(mainTab.m_displayMode == (int)GeoMetryLibrary::DisplayModeTable::base ||
		mainTab.m_displayMode == (int)GeoMetryLibrary::DisplayModeTable::standard ||
		mainTab.m_displayMode == (int)GeoMetryLibrary::DisplayModeTable::all);

	m_config.DISPLAY_MODE = static_cast<GeoMetryLibrary::DisplayModeTable>(mainTab.m_displayMode);

	m_config.Show_INFORM01 = mainTab.showinform01.GetCheck() ? true : false;

	if (ENCCommon::DISPLAY_MODE != m_config.DISPLAY_MODE)
	{
		ENCCommon::DISPLAY_MODE = m_config.DISPLAY_MODE;
		bChangeDisplayMode = true;
		bMapRefresh = true;
	}

	if (ENCCommon::DrawingType != m_config.DrawingType)
	{
		ENCCommon::DrawingType = m_config.DrawingType;
		bRebuildPortrayal = true;
		bMapRefresh = true;
	}

	if (ENCCommon::SAFETY_CONTOUR != m_config.SAFETY_CONTOUR)
	{
		ENCCommon::SAFETY_CONTOUR = m_config.SAFETY_CONTOUR;
		theApp.gisLib->S101RebuildPortrayal();
		theApp.pView->MapRefresh();
	}

	if (ENCCommon::SHALLOW_CONTOUR != m_config.SHALLOW_CONTOUR ||
		ENCCommon::DEEP_CONTOUR != m_config.DEEP_CONTOUR)
	{
		ENCCommon::SHALLOW_CONTOUR = m_config.SHALLOW_CONTOUR;
		ENCCommon::DEEP_CONTOUR = m_config.DEEP_CONTOUR;
		bRebuildPortrayal = true;
		bMapRefresh = true;
	}

	if (ENCCommon::AUTOSELECTION_CATALOGUE != m_config.AUTOSELECTION_CATALOGUE)
	{
		ENCCommon::AUTOSELECTION_CATALOGUE = m_config.AUTOSELECTION_CATALOGUE;
	}

	if (ENCCommon::m_eColorTable != m_config.m_eColorTable)
	{
		ENCCommon::m_eColorTable = m_config.m_eColorTable;
		bMapRefresh = true;

		auto lm = theApp.gisLib->GetLayerManager();
		if (nullptr != lm)
		{
			if (m_config.m_eColorTable == GeoMetryLibrary::ColorTable::Day)
			{
				lm->ChangeS100ColorPalette(L"Day");
			}
			else if (m_config.m_eColorTable == GeoMetryLibrary::ColorTable::Dusk)
			{
				lm->ChangeS100ColorPalette(L"Dusk");
			}
			else if (m_config.m_eColorTable == GeoMetryLibrary::ColorTable::Night)
			{
				lm->ChangeS100ColorPalette(L"Night");
			}
		}
	}

	if (ENCCommon::APPLY_SCALE_MIN != m_config.APPLY_SCALE_MIN)
	{
		ENCCommon::APPLY_SCALE_MIN = m_config.APPLY_SCALE_MIN;
		bRebuildPortrayal = true;
		bMapRefresh = true;
	}

	if (ENCCommon::SIMPLIFIED_POINT_SYMBOL != m_config.SIMPLIFIED_POINT_SYMBOL)
	{
		ENCCommon::SIMPLIFIED_POINT_SYMBOL = m_config.SIMPLIFIED_POINT_SYMBOL;
		bRebuildPortrayal = true;
		bMapRefresh = true;
	}

	if (ENCCommon::FULL_SECTORS != m_config.FULL_SECTORS)
	{
		ENCCommon::FULL_SECTORS = m_config.FULL_SECTORS;
		bRebuildPortrayal = true;
		bMapRefresh = true;
	}

	if (ENCCommon::TEXTOUT != m_config.TEXTOUT)
	{
		ENCCommon::TEXTOUT = m_config.TEXTOUT;
		bMapRefresh = true;
	}

	ENCCommon::DEEP_CONTOUR = m_config.DEEP_CONTOUR;
	ENCCommon::OVER_GROUP = m_config.OVER_GROUP;

	std::unordered_map<int, bool>::iterator itor1;
	std::unordered_map<int, bool>::iterator itor2;

	for (itor1 = ENCCommon::objectDisplaySettings.begin(); itor1 != ENCCommon::objectDisplaySettings.end(); itor1++)
	{
		itor2 = m_config.objectDisplaySettings.find(itor1->first);
		if (itor2 != m_config.objectDisplaySettings.end())
		{
			if (itor1->second != itor2->second)
			{
				itor1->second = itor2->second;
				bObjectDisplaySettingChanges = true;
			}
		}
	}

	if (ENCCommon::DISPLAY_FONT_NAME.compare(m_config.DISPLAY_FONT_NAME) != 0
		|| ENCCommon::DISPLAY_FONT_SIZE != m_config.DISPLAY_FONT_SIZE)
	{
		ENCCommon::DISPLAY_FONT_NAME = m_config.DISPLAY_FONT_NAME;
		ENCCommon::DISPLAY_FONT_SIZE = m_config.DISPLAY_FONT_SIZE;

		theApp.gisLib->ChangeDisplayFont();
		bMapRefresh = true;
	}

	// S-111
	if (ENCCommon::S111_SHOW_NODATA != m_config.S111_SHOW_NODATA)
	{
		ENCCommon::S111_SHOW_NODATA = m_config.S111_SHOW_NODATA;
		bMapRefresh = true;
	}

	if (ENCCommon::Show_INFORM01 != m_config.Show_INFORM01)
	{
		ENCCommon::Show_INFORM01 = m_config.Show_INFORM01;
		bMapRefresh = true;
	}

	if (true == bRebuildPortrayal)
	{
		theApp.gisLib->S101RebuildPortrayal();
		theApp.pView->MapRefresh();
	}
	else if (true == bMapRefresh)
	{
		theApp.pView->MapRefresh();
	}

	s101Tab.Apply();

	theApp.SaveSettings();
}

void CConfigrationDlg::SettingLoadFromFile(std::wstring fileName)
{
	std::ifstream ifs;
	ifs.open(fileName, std::ios::in);

	if (!ifs.is_open())
	{
		return;
	}

	std::string strTrue = "true";
	std::string strFalse = "false";
	std::string strLine;

	while (getline(ifs, strLine))
	{
		std::string opt = strLine.substr(0, 1);
		if (opt.compare("*") == 0)
			continue;

		StringTokenizer *pstringTokenizer = new StringTokenizer(strLine, " \t");
		strLine.clear();

		if (pstringTokenizer->hasMoreTokens())
		{
			std::string token = pstringTokenizer->nextToken();

			if (token.compare("APPLY_SCALE_MIN") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						m_config.APPLY_SCALE_MIN = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						m_config.APPLY_SCALE_MIN = false;
					}
				}
			}
			else if (token.compare("DEEP_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.DEEP_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("DISPLAY_MODE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.DISPLAY_MODE = static_cast<GeoMetryLibrary::DisplayModeTable>(atoi(token.c_str()));
				}
			}
			else if (token.compare("DrawingType") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.DrawingType = atoi(token.c_str());
				}
			}
			else if (token.compare("OVER_GROUP") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.OVER_GROUP = _atoi64(token.c_str());
				}
			}
			else if (token.compare("SAFETY_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.SAFETY_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("SHALLOW_CONTOUR") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.SHALLOW_CONTOUR = atof(token.c_str());
				}
			}
			else if (token.compare("DISPLAY_FONT_NAME") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();

					auto fontName = LibMFCUtil::ConvertCtoWC((char*)token.c_str());

					m_config.DISPLAY_FONT_NAME = fontName;

					delete[] fontName;
				}
			}
			else if (token.compare("DISPLAY_FONT_SIZE") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.DISPLAY_FONT_SIZE = atoi(token.c_str());
				}
			}
			else if (token.compare("FULL_SECTORS") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						m_config.FULL_SECTORS = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						m_config.FULL_SECTORS = false;
					}
				}
			}
			else if (token.compare("TEXTOUT") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					if (token.compare(strTrue) == 0)
					{
						m_config.TEXTOUT = true;
					}
					else if (token.compare(strFalse) == 0)
					{
						m_config.TEXTOUT = false;
					}
				}
			}
			else if (token.compare("m_eColorTable") == 0)
			{
				if (pstringTokenizer->hasMoreTokens())
				{
					token = pstringTokenizer->nextToken();
					m_config.m_eColorTable = static_cast<GeoMetryLibrary::ColorTable>(atoi(token.c_str()));
				}
			}
			else if (token.compare("OBJECT_SHOW_SETTING_BEGIN") == 0)
			{
				getline(ifs, strLine);
				while (strLine.compare("OBJECT_SHOW_SETTING_END") != 0)
				{
					int objectCode = 0;
					bool objectValue = 0;

					StringTokenizer *pstringTokenizerObjSettings = new StringTokenizer(strLine, " \t");

					if (pstringTokenizerObjSettings->hasMoreTokens())
					{
						token = pstringTokenizerObjSettings->nextToken();

						objectCode = atoi(token.c_str());
						if (pstringTokenizerObjSettings->hasMoreTokens())
						{
							token = pstringTokenizerObjSettings->nextToken();

							int value = atoi(token.c_str());

							value ? objectValue = true : objectValue = false;
						}
					}

					delete pstringTokenizerObjSettings;

					if (objectCode > 0)
					{
						auto ositor = m_config.objectDisplaySettings.find(objectCode);
						if (ositor != m_config.objectDisplaySettings.end())
						{
							ositor->second = objectValue;
						}
					}
					getline(ifs, strLine);
				}
			}
		}
		delete pstringTokenizer;
	}

	InitControlValues();
}

CConfiguration_1* CConfigrationDlg::GetConfigPage1()
{
	return &mainTab;
}