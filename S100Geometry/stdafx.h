
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN            
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN       
#endif

#include <afx.h>
#include <afxwin.h>        
#include <afxext.h>        
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <windows.h>



#include <gdiPlus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

#ifndef D2_SAFE_RELEASE
#define D2_SAFE_RELEASE

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#endif