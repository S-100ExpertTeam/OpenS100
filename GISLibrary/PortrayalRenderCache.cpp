#include "stdafx.h"
#include "PortrayalRenderCache.h"
#include "D2D1Resources.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

namespace
{
	ID2D1StrokeStyle1* DefaultStroke(GISLibrary::D2D1Resources* d2)
	{
		return d2->D2D1StrokeStyleGroup.empty() ? nullptr : d2->D2D1StrokeStyleGroup.at(0);
	}
}

namespace GISLibrary
{
	namespace PortrayalRender
	{
		void Prepare(PortrayalCatalogue* pc, D2D1Resources* d2)
		{
			if (!pc || !d2 || !d2->pD2Factory)
			{
				return;
			}
			pc->CreateSVGD2Geometry(d2->pD2Factory);
			pc->CreatePatternImages(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2));
			pc->CreateLineImages(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2));
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName, D2D1Resources* d2)
		{
			if (!pc || !d2 || !d2->pD2Factory)
			{
				return;
			}
			pc->ChangePalette(paletteName, d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2));
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName, D2D1Resources* d2)
		{
			ChangePalette(pc, pugi::as_utf8(paletteName), d2);
		}
	}
}
