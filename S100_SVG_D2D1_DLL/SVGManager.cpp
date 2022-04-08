#include "stdafx.h"

#include "SVGManager.h"
#include "SVG.h"
#include "CSS.h"
#include "S100_Pattern.h"


#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <math.h>

#define M_PI       3.14159265358979323846

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace S100_SVG_D2D1_DLL
{
	ID2D1Factory1* SVGManager::m_pDirect2dFactory = nullptr;
	ID2D1DCRenderTarget* SVGManager::m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* SVGManager::m_pSolidColorBrush = nullptr;
	ID2D1StrokeStyle1* SVGManager::m_pStrokeStyle = nullptr;

	SVGManager::SVGManager()
	{
		
	}

	SVGManager::SVGManager(std::wstring path, std::wstring paletteName, S100_ColorProfile *colorProfile) : SVGManager()
	{
		//Read and add symbols that can be drawn.
		GetSVGFilesByPugiXML(path, paletteName, colorProfile);

		//Read the line style and add it.
		lineMng.GetLineFiles(path);

		//Read and add the color Profile.xml file.
		lineMng.GetColorInfoByPugi(path);

	}

	SVGManager::~SVGManager()
	{
		for each (S100_Pattern* var in patList)
		{
			delete var;
		}
		patList.clear();

		std::unordered_map<std::wstring, Symbol>::iterator itor2;

		for (itor2 = symMap.begin(); itor2 != symMap.end(); itor2++)
		{
			for each (SymbolPath var in itor2->second.symPath)
			{
				var.geom->Release();
			}
			for each (SymbolCircle var in itor2->second.symCircle)
			{
				var.geom->Release();
			}
		}
		symMap.clear();

		for (auto itor = m_svgMap.begin(); itor != m_svgMap.end(); itor++)
		{
			delete itor->second;
		}
	}

	void SVGManager::GetSVGFilesByPugiXML(std::wstring svgFolderPath, std::wstring paletteName, S100_ColorProfile* colorProfile)
	{
		CFileFind  finder;
		std::wstring svgPath = svgFolderPath + _T("Symbols\\*.svg");
		BOOL bWorking = finder.FindFile(svgPath.c_str());

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDots() && !finder.IsDirectory())
			{
				std::wstring filepath = svgFolderPath + _T("Symbols\\") + std::wstring(finder.GetFileName());
				std::wstring svgName = finder.GetFileName().Left(finder.GetFileName().GetLength() - 4);

				if (symMap.find(svgName) == symMap.end())
				{
					SVG* pSvg = new SVG();
					pSvg->ReadSVGFileByPugiXml(filepath);
					pSvg->m_svgName = svgName;
					m_svgMap[svgName.c_str()] = pSvg;
					symMap.insert({ svgName.c_str(), pSvg->GetSymbol(m_pDirect2dFactory, paletteName, colorProfile) });
				}
			}
		}
	}

	HRESULT SVGManager::D2_SetFactory()
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
		return hr;
	}

	HRESULT SVGManager::D2_SetTarget()
	{
		HRESULT hr = S_FALSE;

		if (m_pDirect2dFactory)
		{
			D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_IGNORE),
				0,
				0,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT
			);

			hr = m_pDirect2dFactory->CreateDCRenderTarget(
				&props,
				&m_pRenderTarget);

		}

		return hr;
	}

	HRESULT SVGManager::D2_SetBrush(D2D1::ColorF color)
	{
		HRESULT hr = S_FALSE;
		if (m_pRenderTarget)
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(color),
				&m_pSolidColorBrush);
		}

		return hr;
	}

	void SVGManager::DiscardDeviceResources()
	{
		if (m_pSolidColorBrush)
		{
			SafeRelease(&m_pSolidColorBrush);

		}

	}

	void SVGManager::D2PaintSVG() //Draw svg file.
	{
		if (symList.size() == 0)
		{
			return;
		}

		m_pRenderTarget->BeginDraw(); //Drawing start
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		for (unsigned i = 0; i < symList.size(); i++)
		{
			DrawRealSymbol(symList.at(i));
		}

		m_pRenderTarget->EndDraw(); //Drawing End
	}


	void SVGManager::D2PaintSVG(PortrayalCatalogue *pc)
	{
		if (symList.size() == 0)
		{
			return;
		}

		for (unsigned i = 0; i < symList.size(); i++)
		{
			DrawRealSymbol(symList.at(i), pc);
		}
	}


	void SVGManager::DrawRealSymbol(RealSymbol& symbol)
	{
		Symbol *locsym = nullptr;
		if (symbol.svgManager)
		{
			locsym = symbol.svgManager->GetSymbol(symbol.symName);
		}
		else
		{
			locsym = GetSymbol(symbol.symName);
		}

		const D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(symbol.rotation);

		const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(
			symbol.x / symbol.scale, symbol.y / symbol.scale);

		const D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(symbol.scale * 2, symbol.scale));

		m_pRenderTarget->SetTransform(rot * trans * scale);

		if (locsym)
		{

			for each (SymbolPath sym in locsym->symPath)
			{
				m_pSolidColorBrush->SetColor(D2D1::ColorF(sym.color.c.ToCOLORREF()));
				m_pSolidColorBrush->SetOpacity(sym.color.opacity);

				if (sym.color.fill == true)
				{
					m_pRenderTarget->FillGeometry(sym.geom, m_pSolidColorBrush);
				}
				else
				{
					m_pRenderTarget->DrawGeometry(sym.geom, m_pSolidColorBrush, (FLOAT)(sym.width * 3.7795275591));
				}
			}

			for each (SymbolCircle sym in locsym->symCircle)
			{
				m_pSolidColorBrush->SetColor(D2D1::ColorF(sym.color.c.ToCOLORREF()));
				m_pSolidColorBrush->SetOpacity(sym.color.opacity);
				if (sym.color.fill == true)
				{
					m_pRenderTarget->FillGeometry(sym.geom, m_pSolidColorBrush);
				}
				else
				{
					m_pRenderTarget->DrawGeometry(sym.geom, m_pSolidColorBrush);
				}
			}
		}
	}

	void SVGManager::DrawRealSymbol(RealSymbol& symbol, PortrayalCatalogue *pc)
	{
		auto s100PCManager = pc->GetS100PCManager();
		if (s100PCManager)
		{
			ID2D1RenderTarget* pRenderTarget = GetD2RenderTarget();

			D2D1::Matrix3x2F oldTransform;
			pRenderTarget->GetTransform(&oldTransform);
			
			s100PCManager->Draw(
				symbol.symName,
				pRenderTarget,
				GetD2Brush(),
				GetD2Stroke(),
				D2D1::Point2F(symbol.GetX(), symbol.GetY()),
				symbol.GetRotation(),
				5);

			pRenderTarget->SetTransform(oldTransform);
		}
	}

	void SVGManager::D2PaintSymboledLine()
	{
		m_pRenderTarget->BeginDraw();

		for (unsigned i = 0; i < lineMng.lineList.size(); i++)
		{

			RealLine line = lineMng.lineList.at(i);

			float dgree = atan2(line.startPos.y - line.endPos.y, line.endPos.x - line.startPos.x) * 180 / (float)M_PI;

			D2D1_POINT_2F point = line.startPos;

			//Check the length of the line to be drawn and compare the distance.
			float check = Distance(point, line.endPos) - lineMng.lineMap[line.lineName].length;

			while (check > 0)
			{
				m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				//Draw a line.
				Gdiplus::Color col = RGB(lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].r, lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].g, lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].b);
				D2_SetBrush(col.ToCOLORREF());
				for (unsigned i = 0; i < lineMng.lineMap[line.lineName].dash.size(); i++) {
					D2D1_POINT_2F pf1 = GetMovePoint(point, dgree, lineMng.lineMap[line.lineName].dash.at(i).start);
					D2D1_POINT_2F pf2 = GetMovePoint(pf1, dgree, lineMng.lineMap[line.lineName].dash.at(i).length);

					m_pRenderTarget->DrawLine(
						D2D1::Point2F(pf1.x, pf1.y),
						D2D1::Point2F(pf2.x, pf2.y),
						m_pSolidColorBrush,
						lineMng.lineMap[line.lineName].pen.width
					);
				}

				//Draw a symbol.
				for (unsigned i = 0; i < lineMng.lineMap[line.lineName].sym.size(); i++)
				{
					D2D1_POINT_2F pointf = GetMovePoint(point, dgree, lineMng.lineMap[line.lineName].sym.at(i).position);


					const D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(-dgree);
					const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(
						pointf.x, pointf.y);
					const D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
						D2D1::SizeF(1.0f, 1.0f));
					m_pRenderTarget->SetTransform(rot * trans *scale);

					for each (SymbolPath sym in  symMap[lineMng.lineMap[line.lineName].sym.at(i).reference].symPath)
					{
						D2_SetBrush(sym.color.c.ToCOLORREF());
						m_pSolidColorBrush->SetOpacity(sym.color.opacity);
						if (sym.color.fill == true)
							m_pRenderTarget->FillGeometry(sym.geom, m_pSolidColorBrush);
						else
							m_pRenderTarget->DrawGeometry(sym.geom, m_pSolidColorBrush, sym.width);
					}

					for each (SymbolCircle sym in  symMap[lineMng.lineMap[line.lineName].sym.at(i).reference].symCircle)
					{
						D2_SetBrush(sym.color.c.ToCOLORREF());
						m_pSolidColorBrush->SetOpacity(sym.color.opacity);
						if (sym.color.fill == true)
							m_pRenderTarget->FillGeometry(sym.geom, m_pSolidColorBrush);
						else
							m_pRenderTarget->DrawGeometry(sym.geom, m_pSolidColorBrush);
					}

				}

				check = check - lineMng.lineMap[line.lineName].length;
				point = GetMovePoint(point, dgree, lineMng.lineMap[line.lineName].length);
			}

			//Draw a line.
			Gdiplus::Color col = RGB(lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].r, lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].g, lineMng.colMap[lineMng.lineMap[line.lineName].pen.col].b);
			D2_SetBrush(col.ToCOLORREF());
			// Draw a line in the remaining spots.
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(point.x, point.y),
				D2D1::Point2F(line.endPos.x, line.endPos.y),
				m_pSolidColorBrush,
				lineMng.lineMap[line.lineName].pen.width
			);

		}
		m_pRenderTarget->EndDraw();
		DiscardDeviceResources();
	}

	void SVGManager::D2PaintBitmap()
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		for (unsigned i = 0; i < patList.size(); i++)
		{
			Symbol selectSym = symMap[patList.at(i)->symName];

			//Create a beatmap target.
			ID2D1BitmapRenderTarget *pCompatibleRenderTarget = NULL;
			m_pRenderTarget->CreateCompatibleRenderTarget(
				D2D1::SizeF(selectSym.width + 10, selectSym.height + 10),
				&pCompatibleRenderTarget
			);

			//The brush you're going to use at Bitmap Target.
			ID2D1SolidColorBrush *pBitmapBrush = NULL;

			pCompatibleRenderTarget->BeginDraw();

			const D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(0);
			const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(
				(selectSym.width + 5) / 2, (selectSym.height + 5) / 2);
			const D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
				D2D1::SizeF(1.0f, 1.0f));
			pCompatibleRenderTarget->SetTransform(rot * trans *scale);

			for each (SymbolPath sym in  selectSym.symPath)
			{
				pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(sym.color.c.ToCOLORREF()), &pBitmapBrush);
				pBitmapBrush->SetOpacity(sym.color.opacity);
				if (sym.color.fill == true)
					pCompatibleRenderTarget->FillGeometry(sym.geom, pBitmapBrush);
				else
					pCompatibleRenderTarget->DrawGeometry(sym.geom, pBitmapBrush, sym.width);
			}

			for each (SymbolCircle sym in  selectSym.symCircle)
			{
				pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(sym.color.c.ToCOLORREF()), &pBitmapBrush);
				pBitmapBrush->SetOpacity(sym.color.opacity);
				if (sym.color.fill == true)
					pCompatibleRenderTarget->FillGeometry(sym.geom, pBitmapBrush);
				else
					pCompatibleRenderTarget->DrawGeometry(sym.geom, pBitmapBrush);
			}


			pCompatibleRenderTarget->EndDraw();

			//The content drawn on the beatmap target is saved in the bitmap.
			ID2D1Bitmap *pGridBitmap = NULL;
			pCompatibleRenderTarget->GetBitmap(&pGridBitmap);

			//A brush to draw with a beatmap..
			ID2D1BitmapBrush *pBitampBrush;
			D2D1_BITMAP_BRUSH_PROPERTIES brushProperties =
				D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP);

			m_pRenderTarget->CreateBitmapBrush(pGridBitmap, brushProperties, &pBitampBrush);

			D2_SetBrush(D2D1::ColorF::Black);

			m_pRenderTarget->DrawGeometry(patList.at(i)->geom, m_pSolidColorBrush);
			m_pRenderTarget->FillGeometry(patList.at(i)->geom, pBitampBrush);

		}

		m_pRenderTarget->EndDraw();
		DiscardDeviceResources();
	}

	void SVGManager::D2PaintText(CRect rectView) 
	{
		m_pRenderTarget->BeginDraw();

		for (unsigned i = 0; i < textList.size(); i++)
		{

			const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(
				textList.at(i).point.x, textList.at(i).point.y);

			m_pRenderTarget->SetTransform(trans);
			D2_SetBrush(D2D1::ColorF::Black);



			WCHAR* str = (WCHAR*)textList.at(i).text.c_str();
			IDWriteFactory* pWriteFactory;
			IDWriteTextFormat* pTextFormat;


			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown **>(&pWriteFactory));
			pWriteFactory->CreateTextFormat(str, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, textList.at(i).fontSize, L"", &pTextFormat);


			m_pRenderTarget->DrawText(str, (UINT32)textList.at(i).text.length(), pTextFormat, D2D1::RectF((FLOAT)rectView.left, (FLOAT)rectView.top, (FLOAT)rectView.right, (FLOAT)rectView.bottom), m_pSolidColorBrush);

		}
		m_pRenderTarget->EndDraw();
	}

	void SVGManager::DrawSymbols()
	{
		D2PaintSVG();
		symList.clear();
	}

	void SVGManager::DrawSymbols(PortrayalCatalogue *pc)
	{
		D2PaintSVG(pc);
		symList.clear();
	}

	void SVGManager::PushSymbol(std::wstring _symName, D2D_POINT_2F _position, float _rotation, float _scale)
	{
		RealSymbol rs;
		rs.symName = _symName;
		rs.rotation = _rotation;
		rs.x = _position.x;
		rs.y = _position.y;
		rs.scale = _scale;

		symList.push_back(rs);
	}

	void SVGManager::SetTarget(CDC* pDC, CRect rectView)
	{
		m_pRenderTarget->BindDC(pDC->GetSafeHdc(), rectView);
	}

	D2D1_POINT_2F SVGManager::GetMovePoint(D2D1_POINT_2F point, float dgree, float distance) {

		float dg = dgree * 3.14f / 180;

		float x = point.x + distance * cos(dg);
		float y = point.y - distance * sin(dg);

		D2D1_POINT_2F p;
		p.x = x;
		p.y = y;
		return p;
	}

	float SVGManager::Distance(D2D1_POINT_2F p1, D2D1_POINT_2F p2) {

		float distance;

		distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

		return distance;
	}

	ID2D1PathGeometry* SVGManager::getPatternGeom(D2D1_POINT_2F* points, int pointSize) {

		D2D1_POINT_2F* var = new D2D1_POINT_2F[pointSize];

		for (int i = 0; i < pointSize; i++) {
			var[i] = points[i];
		}
		ID2D1PathGeometry* geom;
		m_pDirect2dFactory->CreatePathGeometry(&geom);

		ID2D1GeometrySink* arrowSink;
		geom->Open(&arrowSink);

		arrowSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		arrowSink->BeginFigure(
			var[0],
			D2D1_FIGURE_BEGIN_FILLED);

		arrowSink->AddLines(var, pointSize);

		arrowSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		arrowSink->Close();
		SafeRelease(&arrowSink);
		delete var;

		return geom;
	}

	Symbol* SVGManager::GetSymbol(std::wstring name)
	{
		auto symbol = symMap.find(name);
		if (symbol != symMap.end())
		{
			return &symbol->second;
		}

		return NULL;
	}

	void SVGManager::SetD2Resource(
		ID2D1Factory1* factory,
		ID2D1DCRenderTarget* renderTarget,
		ID2D1SolidColorBrush* brush,
		ID2D1StrokeStyle1* stroke)
	{
		SetD2Factory(factory);
		SetD2RenderTarget(renderTarget);
		SetD2Brush(brush);
		SetD2Stroke(stroke);
	}

	void SVGManager::ChangeColorPalette(std::wstring paletteName, S100_ColorProfile *colorProfile)
	{
		symMap.clear();
		for (auto i = m_svgMap.begin(); i != m_svgMap.end(); i++)
		{
			auto svg = i->second;
			symMap.insert({ i->first, svg->GetSymbol(m_pDirect2dFactory, paletteName, colorProfile) });
		}
	}

	void SVGManager::SetD2Factory(ID2D1Factory1* value)
	{
		m_pDirect2dFactory = value;
	}

	ID2D1Factory1* SVGManager::GetD2Factory()
	{
		return m_pDirect2dFactory;
	}

	void SVGManager::SetD2RenderTarget(ID2D1DCRenderTarget* value)
	{
		m_pRenderTarget = value;
	}

	ID2D1DCRenderTarget* SVGManager::GetD2RenderTarget()
	{
		return m_pRenderTarget;
	}

	void SVGManager::SetD2Brush(ID2D1SolidColorBrush* value)
	{
		m_pSolidColorBrush = value;
	}

	ID2D1SolidColorBrush* SVGManager::GetD2Brush()
	{
		return m_pSolidColorBrush;
	}

	void SVGManager::SetD2Stroke(ID2D1StrokeStyle1* value)
	{
		m_pStrokeStyle = value;
	}

	ID2D1StrokeStyle1* SVGManager::GetD2Stroke()
	{
		return m_pStrokeStyle;
	}
}