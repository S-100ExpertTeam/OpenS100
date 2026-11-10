#include "stdafx.h"
#include "CoverageDataset.h"

#include "D2D1Resources.h"
#include "D2DView.h"
#include "GF_FeatureType.h"
#include "Layer.h"
#include "PathUtil.h"
#include "ProcessS101.h"
#include "host_functions.h"
#include "S101LuaScriptingReference.h"

#include "../Coverage/Hdf5Coverage.h"
#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_ColorProfile.h"
#include "../PortrayalCatalogue/S100_Item.h"
#include "../PortrayalCatalogue/S100_Palette.h"

#include <chrono>
#include <format>

namespace GISLibrary
{
	namespace
	{
		std::string utf8(const std::wstring& text)
		{
			const std::u8string converted = std::filesystem::path(text).u8string();
			return std::string(converted.begin(), converted.end());
		}

		std::string paletteName()
		{
			switch (ENCCommon::m_eColorTable)
			{
			case GeoMetryLibrary::ColorTable::Dusk: return "Dusk";
			case GeoMetryLibrary::ColorTable::Night: return "Night";
			default: return "Day";
			}
		}

		// The depth settings S-102 shares with S-101, by context parameter name.
		std::vector<std::pair<std::string, std::string>> depthSettings()
		{
			const auto text = [](double value) { return std::format("{}", value); };
			return {
				{ "SafetyContour", text(ENCCommon::SAFETY_CONTOUR) },
				{ "ShallowContour", text(ENCCommon::SHALLOW_CONTOUR) },
				{ "DeepContour", text(ENCCommon::DEEP_CONTOUR) },
			};
		}

		// Straight RGBA bytes to the premultiplied BGRA a Direct2D bitmap takes.
		std::vector<std::uint32_t> toPremultipliedBgra(std::span<const std::uint32_t> pixels)
		{
			std::vector<std::uint32_t> out(pixels.size());
			for (std::size_t i = 0; i < pixels.size(); ++i)
			{
				const std::uint32_t p = pixels[i];
				const std::uint32_t a = p >> 24;
				const std::uint32_t r = (p & 0xFF) * a / 255;
				const std::uint32_t g = ((p >> 8) & 0xFF) * a / 255;
				const std::uint32_t b = ((p >> 16) & 0xFF) * a / 255;
				out[i] = b | (g << 8) | (r << 16) | (a << 24);
			}
			return out;
		}
	}

	CoverageDataset::CoverageDataset()
	{
		type = S100SpatialObjectType::Coverage;
		m_FileType = S100_FileType::FILE_S_100_GRID_H5;
		m_ObejctType = SpatialObjectType::S100H5;
	}

	CoverageDataset::~CoverageDataset()
	{
		ReleaseImages();
	}

	bool CoverageDataset::Open(const std::wstring& path)
	{
		SetFilePath(path);
		SetFileName(s100::path::Stem(path));
		try
		{
			source_ = s100::coverage::Hdf5Coverage::open(path);
		}
		catch (const std::exception&)
		{
			return false;
		}

		features_.clear();
		for (const auto& feature : source_->dataset().features)
		{
			auto featureType = std::make_unique<GF::FeatureType>();
			featureType->id = feature.id;
			featureType->code = feature.code;
			features_.push_back(std::move(featureType));
		}

		const auto& bounds = source_->dataset().bounds;
		MBR mbr(bounds.minLon, bounds.minLat, bounds.maxLon, bounds.maxLat);
		projection(mbr);
		if (auto* layer = GetLayer())
		{
			layer->SetMBR(mbr);
		}
		return true;
	}

	GF::FeatureType* CoverageDataset::GetFeatureTypeByIndex(int index)
	{
		return index >= 0 && index < static_cast<int>(features_.size()) ? features_[static_cast<std::size_t>(index)].get() : nullptr;
	}

	GF::FeatureType* CoverageDataset::GetFeatureType(std::string id)
	{
		for (const auto& feature : features_)
		{
			if (feature->id == id) return feature.get();
		}
		return nullptr;
	}

	bool CoverageDataset::Portray(FeatureCatalogue* fc, PortrayalCatalogue* pc)
	{
		ReleaseImages();
		fillsByFeature_.clear();
		fills_.clear();
		if (!source_ || !fc || !pc || pc->GetRuleFileFormat() != Portrayal::FileFormat::LUA || !pc->GetMainRuleFile())
		{
			return false;
		}

		// The colour profile: palettes of token -> sRGB.
		palettes_.clear();
		if (auto* profiles = pc->GetColorProfiles(); profiles && profiles->GetColorProfilesVector() && !profiles->GetColorProfilesVector()->empty())
		{
			S100_ColorProfile profile;
			const std::wstring file = pc->GetRootPathW() + L"ColorProfiles\\" + profiles->GetColorProfilesVector()->front()->GetFileNameW();
			if (profile.ReadFile(file))
			{
				for (const auto& [name, palette] : profile.Getpalettes())
				{
					auto& colors = palettes_[name];
					for (const auto& [token, item] : palette->GetItem())
					{
						if (item && item->GetSRGB())
						{
							colors[token] = s100::coverage::Rgb{ static_cast<std::uint8_t>(item->GetSRGB()->GetRed()),
								static_cast<std::uint8_t>(item->GetSRGB()->GetGreen()), static_cast<std::uint8_t>(item->GetSRGB()->GetBlue()) };
						}
					}
				}
			}
		}

		const std::wstring mainRule = pc->GetRootPathW() + L"Rules\\" + pc->GetMainRuleFile()->GetFileNameW();
		try
		{
			resultDrawingInstructions.clear();
			ProcessS101::InitPortrayal(utf8(mainRule).c_str(), this, fc, pc);
			if (auto* context = pc->GetContext())
			{
				for (const auto& [name, value] : depthSettings())
				{
					if (context->GetContextParameter(name))
					{
						ProcessS101::PortrayalSetContextParameter(name.c_str(), value.c_str());
					}
				}
			}
			for (const auto& result : *KRS_LUA_SCRIPT_REFERENCE::GetResultDrawingInstructions())
			{
				for (auto& instruction : s100::coverage::parseCoverageInstructions(result.drawingInstructions))
				{
					fills_.push_back(instruction);
					fillsByFeature_.push_back({ result.featureID, std::move(instruction) });
				}
			}
			resultDrawingInstructions.clear();
		}
		catch (const std::exception&)
		{
			resultDrawingInstructions.clear();
			fills_.clear();
			fillsByFeature_.clear();
			return false;
		}
		return !fills_.empty();
	}

	s100::coverage::Rgb CoverageDataset::ResolveColor(std::string_view token) const
	{
		const auto palette = palettes_.find(paletteName());
		const auto& colors = palette != palettes_.end() ? palette->second : (palettes_.empty() ? decltype(palette->second){} : palettes_.begin()->second);
		const auto found = colors.find(std::string(token));
		return found != colors.end() ? found->second : s100::coverage::Rgb{};
	}

	void CoverageDataset::ReleaseImages()
	{
		for (auto& [id, image] : images_)
		{
			if (image.bitmap)
			{
				image.bitmap->Release();
				image.bitmap = nullptr;
			}
		}
		images_.clear();
	}

	void CoverageDataset::DrawCoverage(D2D1Resources* d2, HDC& hDC, Scaler* scaler)
	{
		if (!source_ || !d2 || !scaler || fillsByFeature_.empty())
		{
			return;
		}
		auto* rt = d2->RenderTarget();
		if (!rt)
		{
			return;
		}
		const RECT screen = D2DView::ScreenRECT(*scaler);
		if (FAILED(rt->BindDC(hDC, &screen)))
		{
			return;
		}
		rt->BeginDraw();
		rt->SetTransform(D2D1::Matrix3x2F::Identity());

		const auto& dataset = source_->dataset();
		const auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		for (const Fill& fill : fillsByFeature_)
		{
			const auto* feature = dataset.feature(fill.featureId);
			if (!feature)
			{
				continue;
			}
			const auto component = feature->componentIndex(fill.instruction.attributeCode);
			const auto timeIndex = s100::coverage::displayTimeIndex(feature->timeGroups, now);
			if (!component || !timeIndex)
			{
				continue;
			}
			const std::string key = std::format("{}|{}|{}", paletteName(), *timeIndex, source_->revision());
			Image& image = images_[fill.featureId];
			if (image.key != key || !image.raster)
			{
				if (image.bitmap)
				{
					image.bitmap->Release();
					image.bitmap = nullptr;
				}
				const auto values = source_->readValues(feature->id, { .timeIndex = *timeIndex, .component = *component });
				if (!values)
				{
					continue;
				}
				const auto colorOf = s100::coverage::lookupCellColor(fill.instruction.lookup,
					[this](std::string_view token) { return ResolveColor(token); });
				image.raster = std::make_unique<s100::coverage::CoverageRaster>(feature->grid, *values,
					feature->components[*component].fillValue, colorOf);
				image.key = key;
			}
			const auto& raster = *image.raster;
			if (raster.width() <= 0 || raster.height() <= 0)
			{
				continue;
			}
			if (!image.bitmap)
			{
				const auto bgra = toPremultipliedBgra(raster.pixels());
				const D2D1_BITMAP_PROPERTIES properties = D2D1::BitmapProperties(
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
				if (FAILED(rt->CreateBitmap(D2D1::SizeU(static_cast<UINT32>(raster.width()), static_cast<UINT32>(raster.height())),
					bgra.data(), static_cast<UINT32>(raster.width()) * 4, properties, &image.bitmap)))
				{
					image.bitmap = nullptr;
					continue;
				}
			}

			// The image's corners on the screen: its extent is linear in Mercator, as the world is.
			const auto& extent = raster.extent();
			double west = extent.west;
			double north = s100::coverage::inverseMercatorY(extent.north);
			double east = extent.east;
			double south = s100::coverage::inverseMercatorY(extent.south);
			projection(west, north);
			projection(east, south);
			long left = 0, top = 0, right = 0, bottom = 0;
			scaler->WorldToDevice(west, north, &left, &top);
			scaler->WorldToDevice(east, south, &right, &bottom);
			rt->DrawBitmap(image.bitmap,
				D2D1::RectF(static_cast<float>(left), static_cast<float>(top), static_cast<float>(right), static_cast<float>(bottom)),
				1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		}
		if (rt->EndDraw() == static_cast<HRESULT>(D2DERR_RECREATE_TARGET))
		{
			ReleaseImages();
		}
	}
}
