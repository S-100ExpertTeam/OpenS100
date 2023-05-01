// LibMFCUtil.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LibMFCUtil.h"

#include <algorithm>
#include <regex>
#include <gdiplus.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEG2RAD (M_PI / 180.0); 
#define RAD2DEG (180.0 / M_PI);

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CLibMFCUtilApp

BEGIN_MESSAGE_MAP(CLibMFCUtilApp, CWinApp)
END_MESSAGE_MAP()


// CLibMFCUtilApp construction

CLibMFCUtilApp::CLibMFCUtilApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLibMFCUtilApp object

CLibMFCUtilApp theApp;


// CLibMFCUtilApp initialization

BOOL CLibMFCUtilApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

char* LibMFCUtil::ConvertWCtoC(wchar_t* str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Declaration of char* variable to return
	char* pStr = nullptr;

	// Find the length of the input wchar_t variable.
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	if (strSize <= 0)
	{
		//OutputDebugString(_T("Failed to WideCharToMultiByte()\n"));
		return nullptr;
	}

	// Char* Memory allocation.
	pStr = new char[strSize];

	// Change Type 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}


wchar_t* LibMFCUtil::ConvertCtoWC(char* str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//Wchar_t type variable declaration
	wchar_t* pStr = nullptr;
	//Multi-byte size calculation length return
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	if (strSize <= 0)
	{
		//OutputDebugString(_T("Failed to MultiByteToWideChar()\n"));
		return nullptr;
	}

	//Wchar_t memory allocation
	pStr = new WCHAR[strSize];
	// change type
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;

}

//Extract extensions: Modify the entire path and name+extensions to work.
CString LibMFCUtil::GetExtension(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString ext = _T("");

	int length = path.GetLength();

	if (length <= 0) 
		return ext;

	int indexOfLastReverseSlash = path.ReverseFind('\\'); 
	if (indexOfLastReverseSlash >= 0)// if load full path
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			int lengthOfExtension = path.GetLength() - indexOfLastDot - 1;
			ext = path.Right(lengthOfExtension);
			return ext;
		}
	}
	else // if load name and file Extensions
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			int lengthOfExtension = path.GetLength() - indexOfLastDot - 1;
			ext = path.Right(lengthOfExtension);
			return ext;
		}
	}

	
	return ext;
}


CString LibMFCUtil::GetFileName(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString name = path;

	int length = path.GetLength();

	if (length <= 0)
		return name;

	int indexOfLastReverseSlash = path.ReverseFind('\\');
	if (indexOfLastReverseSlash >= 0)
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			return path.Mid(indexOfLastReverseSlash + 1, indexOfLastDot - indexOfLastReverseSlash - 1);
		}
	}
	else
	{
		//13
		int indexOfLastDot = path.ReverseFind('.');
		int nameLength = path.GetLength();
		if (indexOfLastDot >= 0)
		{
			return path.Mid(0, indexOfLastDot);
		}
	}

	return name;
}


CString LibMFCUtil::GetFolderPathFromFilePath(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFile file;
	if (file.Open(path, CFile::modeRead ))
	{
		CT2CA pszConvertedAnsiString(file.GetFilePath());
		std::string s(pszConvertedAnsiString);
		int itor = (int)s.find_last_of('\\');
		std::string result = s.substr(0,itor)+"\\";

		CString resultC(result.c_str());
		return resultC;
		return file.GetFilePath();
	}
	return _T("");
}

std::string LibMFCUtil::WStringToString(std::wstring str) 
{
	auto chars = ConvertWCtoC((wchar_t*)str.c_str());
	std::string result(chars);
	delete[] chars;
	return result;
}

std::wstring LibMFCUtil::StringToWString(std::string str)
{
	auto wchars = ConvertCtoWC((char*)str.c_str());
	std::wstring result(wchars);
	delete[] wchars;
	return result;
}

POINT LibMFCUtil::RotatePoint(POINT& pt, double radian, int centerX, int centerY)
{
	POINT newPt;
	newPt.x = (LONG)(centerX + (pt.x - centerX) * cos(radian) - (pt.y - centerY) * sin(radian));
	newPt.y = (LONG)(centerY + (pt.x - centerX) * sin(radian) - (pt.y - centerY) * cos(radian));
	return newPt;
}


RECT LibMFCUtil::RotateRect(RECT& rect, double radian, int centerX, int centerY)
{
	RECT newRect;

	POINT pt;
	pt.x = rect.left;
	pt.y = rect.top;

	pt = RotatePoint(pt, radian, centerX, centerY);
	newRect.left = pt.x;
	newRect.top = pt.y;

	pt.x = rect.right;
	pt.y = rect.bottom;

	pt = RotatePoint(pt, radian, centerX, centerY);
	newRect.right= pt.x;
	newRect.bottom = pt.y;

	return newRect;
}

void LibMFCUtil::OutputDebugLongString(CString path)
{
	int count = path.GetLength() / 4096;
	int mod = path.GetLength() % 4096;

	for (int i = 0; i < count; i++)
	{
		CString str = path.Mid(i * 4096, 4096);
		//OutputDebugString(str);
	}

	if (mod > 0)
	{
		//OutputDebugString(path.Mid(count * 4096, mod));
	}
}