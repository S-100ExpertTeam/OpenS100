
// S100Test.h : main header file for the S100Test application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "Class_S101_Base.h"
#include "Class_S101_InputXML.h"
#include "S100ColorProfile.h"
#include "S100DatasetManager.h"
#include "S100PCManager.h"
#include "S100Engine.h"

class DisplayList;

class CS100TestApp : public CWinAppEx
{
public:
	CS100TestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void OpenS101();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	S100Engine s100Engine;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CS100TestApp theApp;
