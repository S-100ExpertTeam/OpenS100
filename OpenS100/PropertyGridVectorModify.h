#pragma once
class CPropertyGridVectorModify : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CPropertyGridVectorModify)

public:
	CPropertyGridVectorModify();
	virtual ~CPropertyGridVectorModify();
	CPropertyGridVectorModify(const CPropertyGridVectorModify& obj)
	{
		*this = obj;
		pSelectedProperty = NULL;
	}

	void operator=(const CPropertyGridVectorModify& obj)
	{

	}
public:
	CMFCPropertyGridProperty *pSelectedProperty;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAddVector();
	afx_msg void OnDeleteVector();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};