#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN            
#include <afxwin.h>        
#include <afxext.h>        
#include <windows.h>

#import <msxml4.dll>
#include "msxml.h"

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

#import <msxml4.dll>
#include "msxml.h"