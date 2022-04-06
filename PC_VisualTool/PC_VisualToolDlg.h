
// PC_VisualToolDlg.h : header file
//

#pragma once
#include <vector>

#include "../PortrayalCatalogue/PortrayalCatalogue.h"


// CPCVisualToolDlg dialog
class CPCVisualToolDlg : public CDialogEx
{
// Construction
public:
	PortrayalCatalogue* m_PC;
	CTreeCtrl m_TreeCtrl;

	void XMLreadFile(CString filepath);
	void SetXmlTreeControl(PortrayalCatalogue* m_PC);
	void SetTreeCtrlColorProfile(std::vector< S100_ColorProfile*>* vecColorProfiles);
	void SetTreeCtrlSymbols(std::vector< ExternalFile*>* vecSymbols);
	void SetTreeLinStyles(std::vector< ExternalFile*>* vecLinStyles);
	void SetTreeAreaFills(std::vector< ExternalFile*>*  vecAreaFills);
	void SetTreeViewingGroups(std::vector<ViewingGroup*>* vecViewingGroups);
	void SetTreeFoundationMode(std::vector<ViewingGroup*>* vecFoundationmode);
	void SetTreeViewingGroupLayers(std::vector<ViewingGroupLayer*>* vecViewingGroupLayers);
	void SetTreeDisplayModes(std::vector<DisplayMode*>* vecDisplayModes);
	void SetTreeDisplayPlanes(std::vector< DisplayPlane*>* vecDisplayPlanes);
	void SetTreeContext(std::vector<ContextParameter*>* vecContext);
	void SetTreeRuleFiles(std::vector<S100_RuleFile*>* vecRuleFiles);

	std::wstring ParameterTypeToString(ParameterType val);

	CPCVisualToolDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual	~CPCVisualToolDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PC_VISUALTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBudtton1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
};
