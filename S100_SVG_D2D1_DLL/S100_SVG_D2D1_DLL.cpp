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

	void CS100_SVG_D2D1_DLLApp::PushSymbol(
		std::wstring _symName,
		D2D_POINT_2F _position,
		float _rotation,
		float _scale)
	{
		RealSymbol rs;
		rs.symName = _symName;
		rs.rotation = _rotation;
		rs.x = _position.x;
		rs.y = _position.y;
		rs.scale = _scale;

		theApp.svgMng->symList.push_back(rs);
	}


	void CS100_SVG_D2D1_DLLApp::PushSymbol(
		SVGManager *svgManager,
		std::wstring _symName,
		D2D_POINT_2F _position,
		float _rotation,
		float _scale)
	{
		RealSymbol symbol(_symName, _position.x, _position.y, _scale, _rotation, svgManager);
		theApp.svgMng->symList.push_back(symbol);
	}

	void CS100_SVG_D2D1_DLLApp::DrawSymbols()
	{
		theApp.svgMng->D2PaintSVG();
		theApp.svgMng->symList.clear();
	}

	void CS100_SVG_D2D1_DLLApp::DrawSymbols(PortrayalCatalogue *pc)
	{
		theApp.svgMng->D2PaintSVG(pc);
		theApp.svgMng->symList.clear();
	}

	void CS100_SVG_D2D1_DLLApp::PushSymboledLine(std::wstring _lineName, D2D1_POINT_2F _startPos, D2D1_POINT_2F _endPos)
	{
		RealLine rl;
		rl.lineName = _lineName;
		rl.startPos = _startPos;
		rl.endPos = _endPos;
		theApp.svgMng->lineMng.lineList.push_back(rl);
	}

	void CS100_SVG_D2D1_DLLApp::DrawSymboledLine()
	{
		theApp.svgMng->D2PaintSymboledLine();
		theApp.svgMng->lineMng.lineList.clear();
	}

	void CS100_SVG_D2D1_DLLApp::PushPattern(std::wstring symName, D2D1_POINT_2F* points, int pointSize)
	{
		//Save points and symbols.
		theApp.svgMng->patList.push_back(new S100_Pattern);
		theApp.svgMng->patList.back()->geom = theApp.svgMng->getPatternGeom(points, pointSize);
		theApp.svgMng->patList.back()->symName = symName;
	}

	void CS100_SVG_D2D1_DLLApp::DrawPattern()
	{
		theApp.svgMng->D2PaintBitmap();
		theApp.svgMng->patList.clear();
	}

	void CS100_SVG_D2D1_DLLApp::PushText(std::wstring _text, POINTF _startPos, float _fontSize)
	{
		S100_Text rt;
		rt.text = _text;
		rt.point = _startPos;
		rt.fontSize = _fontSize;
		theApp.svgMng->textList.push_back(rt);
	}

	void CS100_SVG_D2D1_DLLApp::DrawTexts(CRect rectView)
	{
		theApp.svgMng->D2PaintText(rectView);
		theApp.svgMng->textList.clear();
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