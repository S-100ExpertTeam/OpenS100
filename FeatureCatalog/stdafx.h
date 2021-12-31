// stdafx.h 

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS     
#include <afxwin.h>        
#include <afxext.h>         

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>        
#include <afxodlgs.h>      
#include <afxdisp.h>      
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                   
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>          
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "msxml.h"
#import <msxml4.dll>