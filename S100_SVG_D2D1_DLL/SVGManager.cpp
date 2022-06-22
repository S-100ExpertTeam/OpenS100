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

	SVGManager::SVGManager(std::wstring path, std::wstring paletteName) : SVGManager()
	{
		//Read and add symbols that can be drawn.
		GetSVGFilesByPugiXML(path, paletteName);
	}

	SVGManager::~SVGManager()
	{
		//for each (S100_Pattern* var in patList)
		//{
		//	delete var;
		//}
		//patList.clear();

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

	void SVGManager::GetSVGFilesByPugiXML(std::wstring svgFolderPath, std::wstring paletteName)
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
					symMap.insert({ svgName.c_str(), pSvg->GetSymbol(m_pDirect2dFactory, paletteName) });
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
			symMap.insert({ i->first, svg->GetSymbol(m_pDirect2dFactory, paletteName) });
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