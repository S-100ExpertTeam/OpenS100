#pragma once

#include "../GISLibrary/S100SpatialObject.h"

#include "FeatureCodeString.h"

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
	void SetNoGeometryFeatureList(S100SpatialObject* cell);
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