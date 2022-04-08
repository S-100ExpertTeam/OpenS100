#pragma once
#include "S100_LineManager.h"
#include "Symbol.h"
#include "S100_Text.h"
#include "RealSymbol.h"
#include "SVG.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <d2d1_1.h>

class S100_ColorProfile;
class PortrayalCatalogue;

namespace S100_SVG_D2D1_DLL
{
	class S100_Pattern;
	class SVG;
	class CSS;

	class SVGManager
	{
	public:
		SVGManager();
		SVGManager(std::wstring path, std::wstring paletteName, S100_ColorProfile *colorProfile);
		virtual ~SVGManager();

	public:
		S100_LineManager lineMng;

		// Variables for D2.
		static ID2D1Factory1* m_pDirect2dFactory;
		static ID2D1DCRenderTarget* m_pRenderTarget;
		static ID2D1SolidColorBrush* m_pSolidColorBrush;
		static ID2D1StrokeStyle1* m_pStrokeStyle;

		// Variables to store the drawn pattern
		std::vector<S100_Pattern*> patList;

		// Variables to save the drawn text
		std::vector<S100_Text> textList;

		// Variables that contain drawing svg
		std::vector<RealSymbol> symList;

		std::unordered_map<std::wstring, SVG*> m_svgMap;

		//Variables to store svg files.
		std::unordered_map<std::wstring, Symbol> symMap;

	public:
		void SetD2Resource(
			ID2D1Factory1* factory,
			ID2D1DCRenderTarget* renderTarget,
			ID2D1SolidColorBrush* brush,
			ID2D1StrokeStyle1* stroke);

		Symbol* GetSymbol(std::wstring name);

		//Read the svg file in the specified folder...
		void GetSVGFilesByPugiXML(std::wstring svgFolderPath, std::wstring paletteName, S100_ColorProfile *colorProfile);

		//Create a factory.
		HRESULT D2_SetFactory();

		//Target
		HRESULT D2_SetTarget();

		//Brush
		HRESULT D2_SetBrush(D2D1::ColorF color);

		//Release the memory.
		void DiscardDeviceResources();

		//Drawing SVG
		void D2PaintSVG();
		void D2PaintSVG(PortrayalCatalogue *pc);

		//Drawing Line
		void D2PaintSymboledLine();

		//Drawing Bitmap
		void D2PaintBitmap();

		//Drawing Text
		void D2PaintText(CRect rectView);

		//Drawing Symbols Function
		void DrawSymbols();
		void DrawSymbols(PortrayalCatalogue *pc);

		void DrawRealSymbol(RealSymbol& symbol);
		void DrawRealSymbol(RealSymbol& symbol, PortrayalCatalogue *pc);

		//Set Drawing Symbol
		void PushSymbol(std::wstring _symName, D2D_POINT_2F _position, float _rotation = 0.0, float _scale = 1.0);

		void SetTarget(CDC* pDC, CRect rectView);

		//A function that obtains the coordinates of the desired position through the starting point, angle, and distance.
		D2D1_POINT_2F GetMovePoint(D2D1_POINT_2F point, float dgree, float distance);

		//Distance
		float Distance(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

		//Create and store Geom to draw patterns.
		ID2D1PathGeometry* getPatternGeom(D2D1_POINT_2F* points, int PointSize);

		void ChangeColorPalette(std::wstring paletteName, S100_ColorProfile *colorProfile);

		void SetD2Factory(ID2D1Factory1* value);
		ID2D1Factory1* GetD2Factory();

		void SetD2RenderTarget(ID2D1DCRenderTarget* value);
		ID2D1DCRenderTarget* GetD2RenderTarget();

		void SetD2Brush(ID2D1SolidColorBrush* value);
		ID2D1SolidColorBrush* GetD2Brush();

		void SetD2Stroke(ID2D1StrokeStyle1* value);
		ID2D1StrokeStyle1* GetD2Stroke();
	};
}