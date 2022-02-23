#pragma once
#include "FeatureCodeString.h"
class S101Cell;

class CDialogViewNoGeometry : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogViewNoGeometry)

public:
	CDialogViewNoGeometry(CWnd* pParent = nullptr);   
	virtual ~CDialogViewNoGeometry();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NOGEOMETRY };
#endif

public:
	void SetNoGeometryFeatureList(S101Cell* cell);
	std::vector<CFeatureCodeString> ngflist;
	void InitNonGeometryList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	S101Cell* m_cell;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ViewListNoGeometry;
	afx_msg void OnBnClickedOk();
};