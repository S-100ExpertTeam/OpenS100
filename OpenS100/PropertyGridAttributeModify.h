#pragma once
class SpatialObject;
class AttributeBinding;
class S101Cell;

class CPropertyGridAttributeModify : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CPropertyGridAttributeModify)

public:
	CPropertyGridAttributeModify();
	virtual ~CPropertyGridAttributeModify();

	CPropertyGridAttributeModify(const CPropertyGridAttributeModify& obj)
	{
		*this = obj;
	}

	void operator=(const CPropertyGridAttributeModify& obj) {}

public:
	CMFCPropertyGridProperty *pSelectedProperty;
	void SetSpatialObject(SpatialObject *spatialObject);
	SpatialObject *spatialObject = nullptr;

private:
	int ComplexAttriIndexnum = 0;

public:
	S101Cell* GetS101Cell();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); 
	afx_msg void OnDeleteAttribute();

	void ComplexAttriIndexnumSet(int num);
	int ComplexAttriIndexnumGet();

	afx_msg void OnAttributeeditOpen();
	afx_msg void OnAtttributeeditDelete();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};