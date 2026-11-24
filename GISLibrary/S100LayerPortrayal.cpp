#include "stdafx.h"
// The parts of S100Layer that open a dataset for display and draw it.
// Kept apart from S100Layer.cpp, which is the model (catalogues, product number, drawing set).
#include "PathUtil.h"
#include "TextCompare.h"
#include "LayerOpen.h"
#include "S100Layer.h"
#include "GISLibrary.h"
#include "S10XGML.h"
#include "S100H5.h"
#include "CoverageDataset.h"
#include "PortrayalRenderCache.h"

#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

bool GISLibrary::OpenLayer(S100Layer& layer, const std::wstring& _filepath, LayerManager* lm)
{
	const auto extension = s100::path::Extension(_filepath);

	if ((s100::text::CompareNoCase(extension, L"000") >= 0) &&
		(s100::text::CompareNoCase(extension, L"999") <= 0))
	{
		layer.m_spatialObject = new S101Cell();
		layer.m_spatialObject->SetLayer(&layer);

		auto enc = (S101Cell*)layer.m_spatialObject;

		FeatureCatalogue* fc = nullptr;
		PortrayalCatalogue* pc = nullptr;

		if (enc->OpenMetadata(_filepath))
		{
			auto version = enc->GetVersion();
			fc = lm->catalogManager->getFC(layer.GetProductNumber(), enc->GetVersion());
			pc = lm->catalogManager->getPC(layer.GetProductNumber(), enc->GetVersion());

			if (nullptr == fc) {
				fc = lm->catalogManager->getFC(layer.GetProductNumber());
			}

			// A dataset of an edition no portrayal catalogue was made for is drawn with the one there is.
			if (nullptr == pc) {
				pc = lm->catalogManager->getPC(layer.GetProductNumber());
			}

			layer.SetFeatureCatalog(fc);
			layer.SetPC(pc);
		}

		enc->RemoveAll();

		if (fc && pc && !layer.m_spatialObject->Open(_filepath))
		{
			enc->GetVersion();
			delete layer.m_spatialObject;
			layer.m_spatialObject = nullptr;
			return false;
		}

		//enc->SetAllNumericCode(layer.GetFC());
		return true;
	}
	else if (!s100::text::CompareNoCase(extension, L"gml"))
	{
		layer.SetFeatureCatalog(lm->catalogManager->getFC(layer.GetProductNumber()));
		layer.SetPC(lm->catalogManager->getPC(layer.GetProductNumber()));
		if (layer.GetFC()->getProductId().compare("S-101") == 0)
		{
			layer.m_spatialObject = new S101Cell();
		}
		else
		{
			layer.m_spatialObject = new S10XGML();
		}

		layer.m_spatialObject->SetLayer(&layer);

		if (!layer.m_spatialObject->Open(_filepath))
		{
			delete layer.m_spatialObject;
			layer.m_spatialObject = nullptr;
			return false;
		}

		if (layer.GetFC()->getProductId().compare("S-101") != 0)
		{
			auto gml = (S10XGML*)layer.m_spatialObject;
			gml->SetGeometry();
			gml->CalcMBR();
		}
		
		return true;
	}
	else if (!s100::text::CompareNoCase(extension, L"h5"))
	{
		if (layer.GetFC()->getProductId().compare("S-102") == 0 || layer.GetFC()->getProductId().compare("S-104") == 0
			|| layer.GetFC()->getProductId().compare("S-111") == 0)
		{
			// S-102: portrayed by its portrayal catalogue (CoverageFill), drawn as one image. S-104: the
			// water levels S-102 is adjusted by (S-98 D-2), not drawn itself. S-111: the arrows of its
			// portrayal catalogue at the grid points, thinned as the chart is zoomed out.
			auto coverage = new GISLibrary::CoverageDataset();
			layer.m_spatialObject = coverage;
			coverage->SetLayer(&layer);
			if (!coverage->Open(_filepath))
			{
				delete layer.m_spatialObject;
				layer.m_spatialObject = nullptr;
				return false;
			}
			coverage->Portray(layer.GetFC(), layer.GetPC());
			return true;
		}
		else
		{
			layer.m_spatialObject = new S100H5();
		}

		layer.m_spatialObject->SetLayer(&layer);
		if (!layer.m_spatialObject->Open(_filepath))
		{
			delete layer.m_spatialObject;
			layer.m_spatialObject = nullptr;
			return false;
		}

		return true;
	}
	
	return false;
}

bool S100Layer::DrawGrid(GISLibrary::ViewFrame& frame, Scaler* scaler)
{
	if (GetFileType() != S100_FileType::FILE_S_100_GRID_H5 || !m_spatialObject)
	{
		return false;
	}

	if (auto coverage = dynamic_cast<GISLibrary::CoverageDataset*>(m_spatialObject))
	{
		coverage->DrawCoverage(frame, scaler);
		return true;
	}

	return false;
}

