#include "stdafx.h"
// SENC_Color::ChangePallete: looks the colour token up in the Direct2D colour profile of the
// portrayal catalogue. Kept apart from SENC_Color.cpp, which is plain data.
#include "SENC_Color.h"
#include "D2DConvert.h"
#include "PortrayalRenderCache.h"
#include "../S100Engine/S100PCManager.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

void SENC_Color::ChangePallete(PortrayalCatalogue *pc)
{
	RGBColor = GISLibrary::FromD2D(GISLibrary::PortrayalRender::Caches(pc)->GetS100ColorProfile()->GetColor(GetToken()));
	////auto colorProfile = pc->GetColorProfile();
	////if (colorProfile)
	//{
	//	auto paletteName = pc->GetCurrentPaletteName();
	//	auto rgb = colorProfile->GetRGBRef(paletteName, GetToken());
	//	SetRGBColor(rgb);
	//}
}
