// LibMFCUtil.h : main header file for the LibMFCUtil DLL
//
#pragma once
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <vector>
#include <atlimage.h>
#include <string>

// CLibMFCUtilApp
// See LibMFCUtil.cpp for the implementation of this class
//

#ifdef LIBMFC_API_EXPORT
	#define LIBMFC_API __declspec(dllexport)
#else
	#define LIBMFC_API __declspec(dllimport)
#endif

class LIBMFC_API CLibMFCUtilApp : public CWinApp
{

public:
	CLibMFCUtilApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

namespace LibMFCUtil
{
	extern LIBMFC_API char* ConvertWCtoC(wchar_t* str); // need to release the returned string.
	extern LIBMFC_API wchar_t* ConvertCtoWC(char* str); // need to release the returned string.

	extern LIBMFC_API POINT RotatePoint(POINT& pt, double radian, int centerX, int centerY);
	extern LIBMFC_API RECT RotateRect(RECT& rect, double radian, int centerX, int centerY);

	extern LIBMFC_API std::string WStringToString(std::wstring str);
	extern LIBMFC_API std::wstring StringToWString(std::string str);

	// Only extensions are imported from the file path (e.g., txt, jpg).
	extern LIBMFC_API CString GetExtension(CString path);

	// Get only the file name from the file path;
	extern LIBMFC_API CString GetFileName(CString path);

	extern LIBMFC_API CString GetFolderPathFromFilePath(CString path);

	extern LIBMFC_API void OutputDebugLongString(CString path);

	extern LIBMFC_API COLORREF RandomColor(float minRange, float maxRange);
}

