// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Ŭ�����Դϴ�.
#include <afxodlgs.h>       // MFC OLE ��ȭ ���� Ŭ�����Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"

#include "Common.h"
#include <afxcontrolbars.h>

#ifndef _TEXTCLUTTER
	#define _TEXTCLUTTER
#endif


#ifndef _GETOBJECT
	//#define _GETOBJECT
#endif

#ifndef _DRAW_WRECKS_CS
	#define _DRAW_WRECKS_CS
#endif

#ifndef _TEST_PERFORMANCE
	//#define _TEST_PERFORMANCE
#endif

#ifndef _GET_LANDREGION
	//#define _GET_LANDREGION
#endif

#ifndef _GET_ATTRIBUTES
//#define _GET_ATTRIBUTES
#endif
#ifndef _DEBUG_POINTS
//#define _DEBUG_POINTS
#endif

#ifndef _USE_SENC
	#define _USE_SENC
#endif

#ifndef _S101_VIEWER
	//#define _S101_VIEWER
#endif

#ifndef ROUTE_CHECK // GISViewer ROUTE_CHECK �� ���� üũ/�����ϱ�
	#define ROUTE_CHECK
#endif

#ifndef PORTRAYAL_RULE_TEST // GISViewer ROUTE_CHECK �� ���� üũ/�����ϱ�
	//#define PORTRAYAL_RULE_TEST
#endif

#import <msxml4.dll>
#include "msxml.h"

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

#include "..\\LatLonUtility\\LatLonUtility.h"

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