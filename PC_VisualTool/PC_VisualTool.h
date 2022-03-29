
// PC_VisualTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPCVisualToolApp:
// See PC_VisualTool.cpp for the implementation of this class
//

class CPCVisualToolApp : public CWinApp
{
public:
	CPCVisualToolApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPCVisualToolApp theApp;
