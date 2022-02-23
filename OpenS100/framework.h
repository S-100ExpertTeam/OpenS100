#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      

#define _AFX_ALL_WARNINGS

#include <afxwin.h>         
#include <afxext.h>         


#include <afxdisp.h>        



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>          
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>            
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>    


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC // include Microsoft memory leak detection procedures
#define _INC_MALLOC      // exclude standard memory alloc procedures
#endif





#include <gdiPlus.h>
#include <afxcview.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include "Common.h"




#define ZOOM_FACTOR 30

#define WM_TRANSLATION_MESSAGE_TO_PARENT_DELETE_ATTRIBUTE	80001
#define WM_TRANSLATION_MESSAGE_TO_PARENT_ADD_VECTOR			80101
#define WM_TRANSLATION_MESSAGE_TO_PARENT_DELETE_VECTOR		80102
#define WM_TRANSLATION_MESSAGE_TO_PARENT_SELECTED_VECTOR	80103
#define WM_TRANSLATION_MESSAGE_TO_CURRENT_SELECTION_ADD	    80104


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif




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


