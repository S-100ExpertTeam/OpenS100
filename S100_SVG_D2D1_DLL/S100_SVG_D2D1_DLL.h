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

		//Saving symbol information to be drawn
		//(symbol name, position, angle, size)
		void PushSymbol(std::wstring _symName, D2D_POINT_2F _position, float _rotation = 0.0, float _scale = 1.0);

		void PushSymbol(
			SVGManager *svgManager,
			std::wstring _symName, 
			D2D_POINT_2F _position, 
			float _rotation = 0.0, 
			float _scale = 1.0);

		// Save the line information to be drawn
		//(Line name, starting point, ending point)
		void PushSymboledLine(std::wstring _lineName, D2D1_POINT_2F _startPos, D2D1_POINT_2F _endPos);

		//Save pattern information to be drawn
		//(symbol name, coordinates, number of coordinates)
		void PushPattern(std::wstring symName, D2D1_POINT_2F* points, int pointSize);

		//Save the text information to be drawn
		//(String, coordinates, letter size)
		void PushText(std::wstring _text, POINTF _startPos, float _fontSize);

		//Draw a saved symbol
		void DrawSymbols();
		void DrawSymbols(PortrayalCatalogue *pc);

		//Draw a saved line
		void DrawSymboledLine();

		//Draw saved pattern
		void DrawPattern();

		//Drawing the saved text.
		void DrawTexts(CRect rectView);
	};

	CS100_SVG_D2D1_DLLApp* GetApp();
}
