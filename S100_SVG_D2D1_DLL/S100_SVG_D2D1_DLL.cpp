#include "stdafx.h"
#include "S100_SVG_D2D1_DLL.h"
#include "SVGManager.h"
#include "S100_Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

namespace S100_SVG_D2D1_DLL
{
	CS100_SVG_D2D1_DLLApp::CS100_SVG_D2D1_DLLApp()
	{
		svgMng = new SVGManager();
	}

	CS100_SVG_D2D1_DLLApp::~CS100_SVG_D2D1_DLLApp()
	{
		delete svgMng;
		svgMng = nullptr;
	}

	CS100_SVG_D2D1_DLLApp theApp;

	void CS100_SVG_D2D1_DLLApp::SetFactory()
	{
	}

	void CS100_SVG_D2D1_DLLApp::SetTarget(CDC* pDC, CRect rectView)
	{
		theApp.svgMng->m_pRenderTarget->BindDC(pDC->GetSafeHdc(), rectView);
	}

	CS100_SVG_D2D1_DLLApp* GetApp()
	{
		return &theApp;
	}

	SVGManager* CS100_SVG_D2D1_DLLApp::GetSVGManager()
	{
		return svgMng;
	}
}