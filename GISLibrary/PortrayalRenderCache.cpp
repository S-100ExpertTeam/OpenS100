#include "stdafx.h"
#include "PortrayalRenderCache.h"
#include "D2D1Resources.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../S100Engine/S100PCManager.h"

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
			auto manager = pc->GetS100PCManager();
			if (!manager)
			{
				return;
			}
			manager->CreateSVGGeometry(d2->pD2Factory);
			manager->CreateBitmapImage(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2), pc->GetCurrentPaletteName());
			manager->CreateLineImage(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2), pc->GetCurrentPaletteName());
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName, D2D1Resources* d2)
		{
			if (!pc || !d2 || !d2->pD2Factory)
			{
				return;
			}
			pc->ChangePalette(paletteName);
			auto manager = pc->GetS100PCManager();
			if (!manager)
			{
				return;
			}
			manager->DeleteBitmapImage();
			manager->CreateBitmapImage(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2), pc->GetCurrentPaletteName());
			manager->DeleteLineImage();
			manager->CreateLineImage(d2->pD2Factory, d2->pImagingFactory, DefaultStroke(d2), pc->GetCurrentPaletteName());
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName, D2D1Resources* d2)
		{
			ChangePalette(pc, pugi::as_utf8(paletteName), d2);
		}
	}
}
