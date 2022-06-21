#pragma once


#ifndef __AFXWIN_H__
#error "Include 'stdafx.h' before including this file for PCH."
#endif

// CS100_SVG_D2D1_DLLApp
//
#include <string>

//namespace S100_SVG_D2D1_DLL

#ifdef S100_SVG_D2D1_DLL_EXPORT
#define S100_SVG_D2D1_DLL_API __declspec(dllexport)
#else
#define S100_SVG_D2D1_DLL_API __declspec(dllimport)
#endif

class PortrayalCatalogue;
namespace S100_SVG_D2D1_DLL
{
	class SVGManager;
	class CS100_SVG_D2D1_DLLApp
	{
	public:
		CS100_SVG_D2D1_DLLApp();
		virtual ~CS100_SVG_D2D1_DLLApp();
	public:
		SVGManager* GetSVGManager();
		SVGManager *svgMng;

	public:
		void SetFactory();

		void SetTarget(CDC* pDC, CRect rectView);
	};

	CS100_SVG_D2D1_DLLApp* GetApp();
}
