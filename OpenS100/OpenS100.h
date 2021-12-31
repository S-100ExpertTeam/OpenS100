
// OpenS100.h: OpenS100 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif
#include "resource.h" 
#include "DockablePaneLayerManager.h"
#include "DockablePaneCurrentSelection.h"
#include "DockablePaneRelation.h"
#include "DockablePaneLogger.h"
#include "DockablePaneEditWindow.h"
#include "DockablePaneAttributeList.h"

class COpenS100View;
class COpenS100App : public CWinAppEx
{
public:
	COpenS100App() noexcept;
	virtual ~COpenS100App();

public:
	ULONG_PTR gdiplusToken = NULL; // GDI
	COpenS100View* pView = nullptr;
	CDockablePaneLayerManager m_pDockablePaneLayerManager; //Layer Manager
	CDockablePaneCurrentSelection m_DockablePaneCurrentSelection; //Current Selection
	CDockablePaneRelation m_DockablePaneRelation; //Relation
	CDockablePaneEditWindow m_DockablePaneEditWindow; //
	CDockablePaneAttributeList m_DockablePaneAttributeList;
	BOOL  m_bHiColorIcons;

public:
	CDockablePaneAttributeList& GetDockFeatureInformationList();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void LoadSettings();

public:
	// Initial Map Positioning
	void InitStartingLocation();
	void MapRefresh();
	void SaveSettings();
};

extern COpenS100App theApp;