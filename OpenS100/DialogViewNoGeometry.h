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
	void SetNoGeometryFeatureList(S100SpatialObject* s100so);
	std::vector<CFeatureCodeString> ngflist;
	void InitNonGeometryList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	S100SpatialObject* s100so = nullptr;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ViewListNoGeometry;
	afx_msg void OnBnClickedOk();
};