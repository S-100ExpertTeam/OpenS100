#pragma once

#include "afxtabctrl.h"

class CMFCTabCtrlMsgToParent :
	public CMFCTabCtrl
{
public:
	CMFCTabCtrlMsgToParent();
	virtual ~CMFCTabCtrlMsgToParent();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};