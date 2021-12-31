#pragma once

class CTreeCtrlExt : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlExt)

public:
	CTreeCtrlExt();
	virtual ~CTreeCtrlExt();

public:
	CString NodeName;
	HTREEITEM CurrentITEM;
	int CheckOA;
	bool Check;
	CImageList ImgList;

public:
	void AddTreeNode(HTREEITEM *PNode, HTREEITEM *CNode, CString Data, int Color);
	void SetItemFont(HTREEITEM, LOGFONT&);
	void SetItemBold(HTREEITEM, BOOL);
	void SetItemColor(HTREEITEM, COLORREF);
	BOOL GetItemFont(HTREEITEM, LOGFONT *);
	BOOL GetItemBold(HTREEITEM);
	COLORREF GetItemColor(HTREEITEM);
	void SetNodeName(CString nodeName);
	CString GetNodeName();

protected:
	DECLARE_MESSAGE_MAP()
	struct Color_Font {
		COLORREF color;
		LOGFONT  logfont;
	};
	CMap <void*, void*, Color_Font, Color_Font&> m_mapColorFont;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};