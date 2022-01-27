// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
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

#ifndef ROUTE_CHECK // GISViewer ROUTE_CHECK 와 같이 체크/해제하기
	#define ROUTE_CHECK
#endif

#ifndef PORTRAYAL_RULE_TEST // GISViewer ROUTE_CHECK 와 같이 체크/해제하기
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