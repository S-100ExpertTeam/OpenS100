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
#include "../FeatureCatalog/FeatureType.h"
#include "../FeatureCatalog/SimpleAttribute.h"
#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_ColorProfile.h"
#include "../PortrayalCatalogue/S100_Item.h"
#include "../PortrayalCatalogue/S100_Palette.h"

#include <algorithm>
#include <cctype>
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

	namespace
	{
		// The coverage datasets alive now (shown or kept in a cache): where an S-102 dataset finds
		// its water levels. Datasets are created, drawn and deleted on the drawing thread.
		std::set<CoverageDataset*>& registry()
		{
			static std::set<CoverageDataset*> datasets;
			return datasets;
		}

		CoverageDataset::DepthSettings& settings()
		{
			static CoverageDataset::DepthSettings value;
			return value;
		}

		std::uint64_t& settingsVersion()
		{
			static std::uint64_t version = 1;
			return version;
		}

		std::uint64_t& suppressionVersion()
		{
			static std::uint64_t version = 1;
			return version;
		}

		bool isProduct(const s100::coverage::CoverageSource* source, std::string_view product)
		{
			return source && source->dataset().productId == product;
		}
	}

	const CoverageDataset::DepthSettings& CoverageDataset::GetDepthSettings() noexcept
	{
		return settings();
	}

	void CoverageDataset::SetDepthSettings(const DepthSettings& value)
	{
		if (settings().enhancedSafetyContour != value.enhancedSafetyContour)
		{
			++suppressionVersion();
		}
		settings() = value;
		++settingsVersion();
	}

	std::uint64_t CoverageDataset::SuppressionVersion() noexcept
	{
		return suppressionVersion();
	}

	std::vector<std::vector<s100::coverage::GeoPosition>> CoverageDataset::SuppressionRings()
	{
		std::vector<std::vector<s100::coverage::GeoPosition>> rings;
		if (!settings().enhancedSafetyContour)
		{
			return rings;
		}
		for (CoverageDataset* dataset : registry())
		{
			if (!dataset->adjusted_)
			{
				continue;
			}
			if (!dataset->validRings_)
			{
				// Of the file's depths: adjustment never makes a cell valid or empty.
				std::vector<s100::coverage::GridRing> outline;
				for (const auto& feature : dataset->raw_->dataset().features)
				{
					const auto depth = feature.componentIndex("depth");
					if (!depth) continue;
					const auto values = dataset->raw_->readValues(feature.id, { .timeIndex = 0, .component = *depth });
					if (!values) continue;
					auto featureRings = s100::coverage::validGridRings(feature.grid, *values, feature.components[*depth].fillValue);
					std::ranges::move(featureRings, std::back_inserter(outline));
				}
				dataset->validRings_ = std::move(outline);
			}
			for (const auto& ring : *dataset->validRings_)
			{
				rings.push_back(ring.exterior);
			}
		}
		return rings;
	}

	CoverageDataset::WlaStatus CoverageDataset::CurrentWlaStatus()
	{
		WlaStatus status;
		for (CoverageDataset* dataset : registry())
		{
			if (!dataset->adjusted_)
			{
				continue;
			}
			status.applied += dataset->report_.applied;
			status.issues.insert(dataset->report_.issues.begin(), dataset->report_.issues.end());
			if (!status.s102Datum) status.s102Datum = s100::coverage::verticalDatumOf(dataset->raw_->dataset());
		}
		for (CoverageDataset* dataset : registry())
		{
			if (isProduct(dataset->raw_.get(), "S-104") && !status.s104Datum)
			{
				status.s104Datum = s100::coverage::verticalDatumOf(dataset->raw_->dataset());
			}
		}
		return status;
	}

	std::optional<std::pair<s100::coverage::TimePoint, s100::coverage::TimePoint>> CoverageDataset::WaterLevelTimeSpan()
	{
		std::optional<std::pair<s100::coverage::TimePoint, s100::coverage::TimePoint>> span;
		for (CoverageDataset* dataset : registry())
		{
			if (!isProduct(dataset->raw_.get(), "S-104"))
			{
				continue;
			}
			for (const auto& feature : dataset->raw_->dataset().features)
			{
				for (const auto& group : feature.timeGroups)
				{
					if (!group.time) continue;
					if (!span) span = std::pair{ *group.time, *group.time };
					span->first = (std::min)(span->first, *group.time);
					span->second = (std::max)(span->second, *group.time);
				}
			}
		}
		return span;
	}

	CoverageDataset::CoverageDataset()
	{
		type = S100SpatialObjectType::Coverage;
		m_FileType = S100_FileType::FILE_S_100_GRID_H5;
		m_ObejctType = SpatialObjectType::S100H5;
		registry().insert(this);
		++suppressionVersion();
	}

	CoverageDataset::~CoverageDataset()
	{
		registry().erase(this);
		++suppressionVersion();
		ReleaseImages();
		for (ID2D1StrokeStyle* stroke : { s102BoundaryStroke_, wlaBoundaryStroke_ })
		{
			if (stroke) stroke->Release();
		}
	}

	const s100::coverage::CoverageSource* CoverageDataset::Source() const noexcept
	{
		return adjusted_ ? static_cast<const s100::coverage::CoverageSource*>(adjusted_.get()) : raw_.get();
	}

	void CoverageDataset::PrepareAdjustment()
	{
		if (!adjusted_)
		{
			return;
		}
		std::vector<const s100::coverage::CoverageSource*> sources;
		for (CoverageDataset* dataset : registry())
		{
			if (dataset != this && isProduct(dataset->raw_.get(), "S-104"))
			{
				sources.push_back(dataset->raw_.get());
			}
		}
		std::ranges::sort(sources);
		if (adjustedFor_ == settingsVersion() && sources == adjustedSources_)
		{
			return;
		}
		report_ = adjusted_->update(settings().adjustment, sources);
		adjustedFor_ = settingsVersion();
		adjustedSources_ = std::move(sources);
	}

	bool CoverageDataset::Open(const std::wstring& path)
	{
		SetFilePath(path);
		SetFileName(s100::path::Stem(path));
		try
		{
			raw_ = s100::coverage::Hdf5Coverage::open(path);
		}
		catch (const std::exception&)
		{
			return false;
		}
		adjusted_ = isProduct(raw_.get(), "S-102") ? std::make_shared<s100::coverage::WaterLevelAdjustedCoverage>(raw_) : nullptr;
		adjustedFor_ = 0;
		adjustedSources_.clear();
		validRings_.reset();
		++suppressionVersion();

		features_.clear();
		for (const auto& feature : raw_->dataset().features)
		{
			auto featureType = std::make_unique<GF::FeatureType>();
			featureType->id = feature.id;
			featureType->code = feature.code;
			features_.push_back(std::move(featureType));
		}

		const auto& bounds = raw_->dataset().bounds;
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
		if (!raw_ || !fc || !pc || pc->GetRuleFileFormat() != Portrayal::FileFormat::LUA || !pc->GetMainRuleFile())
		{
			return false;
		}
		// S-104 serves as water levels for S-102 and is not drawn (hidden by default, as in opens100-web).
		if (isProduct(raw_.get(), "S-104"))
		{
			return false;
		}

		// The catalogue's names of the features and their values, for the pick report.
		featureNames_.clear();
		attributeNames_.clear();
		for (const auto& feature : raw_->dataset().features)
		{
			if (auto* featureType = fc->GetFeatureType(feature.code))
			{
				featureNames_[feature.code] = featureType->GetName();
			}
			for (const auto& component : feature.components)
			{
				if (auto* attribute = fc->GetSimpleAttribute(component.name))
				{
					attributeNames_[component.name] = attribute->GetName();
				}
			}
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
		lines_.clear();
	}

	void CoverageDataset::DrawLines(D2D1Resources* d2, Scaler* scaler, const s100::coverage::CoverageFeature& feature,
		const s100::coverage::Values& depth, double fillValue)
	{
		const DepthSettings& depthSettings = settings();
		const bool wlaActive = depthSettings.adjustment.wla.kind != s100::coverage::WlaMethod::Kind::Off;
		if ((!depthSettings.enhancedSafetyContour && !wlaActive) || !depth)
		{
			return;
		}
		const std::string key = std::format("{}|{}|{}|{}", ENCCommon::SAFETY_CONTOUR, Source()->revision(),
			depthSettings.enhancedSafetyContour, wlaActive);
		Lines& lines = lines_[feature.id];
		if (lines.key != key)
		{
			const auto* adjustment = adjusted_ ? adjusted_->adjustment(feature.id) : nullptr;
			lines.lines = s100::coverage::depthLines({ .grid = &feature.grid, .depth = *depth, .fillValue = fillValue,
				.applied = adjustment ? std::span<const std::uint8_t>(adjustment->applied) : std::span<const std::uint8_t>(),
				.appliedCount = adjustment ? adjustment->appliedCount : 0,
				.esc = depthSettings.enhancedSafetyContour, .safetyContour = ENCCommon::SAFETY_CONTOUR, .wlaActive = wlaActive });
			lines.key = key;
		}

		auto* rt = d2->RenderTarget();
		auto* brush = d2->SolidColorBrush();
		if (!rt || !brush)
		{
			return;
		}
		// Millimetres at 96 device pixels per inch; Direct2D dashes are in stroke widths.
		constexpr float pixelsPerMillimetre = 96.0f / 25.4f;
		const RECT screen = D2DView::ScreenRECT(*scaler);
		const auto draw = [&](const s100::coverage::GridEdges& edges, const s100::coverage::S98LineStyle& style, const s100::coverage::Rgb& color,
			ID2D1StrokeStyle* stroke) {
			brush->SetColor(D2D1::ColorF(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
			const float width = static_cast<float>(style.width) * pixelsPerMillimetre;
			for (std::size_t i = 0; i + 3 < edges.size(); i += 4)
			{
				double x1 = edges[i], y1 = edges[i + 1], x2 = edges[i + 2], y2 = edges[i + 3];
				projection(x1, y1);
				projection(x2, y2);
				// To the fraction of a pixel, as the image under the lines is placed.
				float sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;
				scaler->WorldToDevice_F(x1, y1, &sx1, &sy1);
				scaler->WorldToDevice_F(x2, y2, &sx2, &sy2);
				if ((sx1 < screen.left && sx2 < screen.left) || (sx1 > screen.right && sx2 > screen.right)
					|| (sy1 < screen.top && sy2 < screen.top) || (sy1 > screen.bottom && sy2 > screen.bottom))
				{
					continue;
				}
				rt->DrawLine(D2D1::Point2F(sx1, sy1), D2D1::Point2F(sx2, sy2), brush, width, stroke);
			}
		};
		const std::string palette = paletteName();
		if (lines.lines.s102Boundary)
		{
			draw(*lines.lines.s102Boundary, s100::coverage::s102BoundaryStyle(), s100::coverage::depwlColor(palette),
				DashedStroke(d2, s100::coverage::s102BoundaryStyle(), s102BoundaryStroke_));
		}
		if (lines.lines.wlaBoundary)
		{
			draw(*lines.lines.wlaBoundary, s100::coverage::wlaBoundaryStyle(), s100::coverage::depwlColor(palette),
				DashedStroke(d2, s100::coverage::wlaBoundaryStyle(), wlaBoundaryStroke_));
		}
		if (lines.lines.safetyContour)
		{
			draw(*lines.lines.safetyContour, s100::coverage::safetyContourStyle(), ResolveColor(s100::coverage::safetyContourStyle().token), nullptr);
		}
	}

	ID2D1StrokeStyle* CoverageDataset::DashedStroke(D2D1Resources* d2, const s100::coverage::S98LineStyle& style, ID2D1StrokeStyle*& cache)
	{
		// Direct2D dashes are in stroke widths.
		if (!cache && d2->Factory())
		{
			std::vector<float> dashes;
			for (const double length : style.dash) dashes.push_back(static_cast<float>(length / style.width));
			d2->Factory()->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER, 10.0f, D2D1_DASH_STYLE_CUSTOM, 0.0f), dashes.data(), static_cast<UINT32>(dashes.size()), &cache);
		}
		return cache;
	}

	void CoverageDataset::DrawCoverage(D2D1Resources* d2, HDC& hDC, Scaler* scaler)
	{
		if (!raw_ || !d2 || !scaler || fillsByFeature_.empty())
		{
			return;
		}
		PrepareAdjustment();
		const s100::coverage::CoverageSource* source = Source();
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

		const auto& dataset = source->dataset();
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
			const std::string key = std::format("{}|{}|{}", paletteName(), *timeIndex, source->revision());
			Image& image = images_[fill.featureId];
			if (image.key != key || !image.raster)
			{
				if (image.bitmap)
				{
					image.bitmap->Release();
					image.bitmap = nullptr;
				}
				const auto values = source->readValues(feature->id, { .timeIndex = *timeIndex, .component = *component });
				if (!values)
				{
					continue;
				}
				const auto colorOf = s100::coverage::lookupCellColor(fill.instruction.lookup,
					[this](std::string_view token) { return ResolveColor(token); });
				// Rows evenly spaced in latitude, as the viewer's equirectangular chart draws them (and
				// the grid lines drawn over the image).
				image.raster = std::make_unique<s100::coverage::CoverageRaster>(feature->grid, *values,
					feature->components[*component].fillValue, colorOf, s100::coverage::RasterProjection::Geographic);
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

			// The image's corners on the screen, to the fraction of a pixel: its rows are linear in
			// latitude, as the world is.
			const auto& extent = raster.extent();
			double west = extent.west;
			double north = raster.latitude(extent.north);
			double east = extent.east;
			double south = raster.latitude(extent.south);
			projection(west, north);
			projection(east, south);
			float left = 0, top = 0, right = 0, bottom = 0;
			scaler->WorldToDevice_F(west, north, &left, &top);
			scaler->WorldToDevice_F(east, south, &right, &bottom);
			rt->DrawBitmap(image.bitmap, D2D1::RectF(left, top, right, bottom), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

			if (fill.instruction.attributeCode == "depth")
			{
				DrawLines(d2, scaler, *feature, source->readValues(feature->id, { .timeIndex = *timeIndex, .component = *component }),
					feature->components[*component].fillValue);
			}
		}
		if (rt->EndDraw() == static_cast<HRESULT>(D2DERR_RECREATE_TARGET))
		{
			ReleaseImages();
		}
	}

	namespace
	{
		// "12.30 m", as opens100-web writes a value: two decimals, the unit after it.
		std::string formatValue(double value, const std::string& uom)
		{
			return uom.empty() ? std::format("{:.2f}", value) : std::format("{:.2f} {}", value, uom);
		}

		// A feature code spaced into words, "BathymetryCoverage" -> "Bathymetry Coverage".
		std::string spacedCode(const std::string& code)
		{
			std::string out;
			for (std::size_t i = 0; i < code.size(); ++i)
			{
				const unsigned char c = static_cast<unsigned char>(code[i]);
				const unsigned char previous = i > 0 ? static_cast<unsigned char>(code[i - 1]) : 0;
				if (i > 0 && std::isupper(c) && (std::islower(previous) || std::isdigit(previous)))
				{
					out.push_back(' ');
				}
				out.push_back(code[i]);
			}
			return out;
		}
	}

	std::optional<CoverageDataset::PickSection> CoverageDataset::PickAt(const s100::coverage::GeoPosition& position)
	{
		if (!raw_ || fillsByFeature_.empty())
		{
			return std::nullopt;
		}
		PrepareAdjustment();
		const auto& dataset = raw_->dataset();
		const auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		PickSection section{ .dataset = dataset.name, .product = dataset.productId };
		std::set<std::string, std::less<>> picked;
		for (const Fill& drawn : fillsByFeature_)
		{
			const auto* feature = dataset.feature(drawn.featureId);
			if (!feature || !picked.insert(drawn.featureId).second)
			{
				continue;
			}
			// The record drawn now, as DrawCoverage chooses it.
			const auto timeIndex = s100::coverage::displayTimeIndex(feature->timeGroups, now);
			const auto cell = s100::coverage::gridCellAt(feature->grid, position);
			if (!timeIndex || !cell)
			{
				continue;
			}
			const std::size_t index = static_cast<std::size_t>(cell->row) * static_cast<std::size_t>(feature->grid.cols)
				+ static_cast<std::size_t>(cell->col);
			std::vector<std::pair<std::string, std::string>> rows;
			for (std::size_t c = 0; c < feature->components.size(); ++c)
			{
				const auto& component = feature->components[c];
				const auto values = raw_->readValues(feature->id, { .timeIndex = *timeIndex, .component = c });
				if (!values || index >= values->size() || s100::coverage::isNoData((*values)[index], component.fillValue))
				{
					continue;
				}
				const double value = (*values)[index];
				// The catalogue's name; failing that, the file's (Group_F); failing that, the code.
				const auto named = attributeNames_.find(component.name);
				rows.emplace_back(named != attributeNames_.end() && !named->second.empty() ? named->second
					: component.label.empty() ? component.name : component.label, formatValue(value, component.uom));

				double portrayed = value;
				const auto* adjustment = component.name == "depth" && adjusted_ ? adjusted_->adjustment(feature->id) : nullptr;
				if (adjustment && adjustment->depth && index < adjustment->depth->size())
				{
					portrayed = (*adjustment->depth)[index];
					// S-98 D-2 item 3b: both the unadjusted and the adjusted depth where WLA applies.
					if (index < adjustment->applied.size() && adjustment->applied[index] == 1)
					{
						rows.emplace_back("Water level", formatValue(adjustment->waterLevel[index], "m"));
						rows.emplace_back("Depth (adjusted)", formatValue(portrayed, component.uom) + " ["
							+ s100::coverage::wlaIndication(settings().adjustment.wla).value_or("WLA") + "]");
					}
					else if (portrayed != value)
					{
						rows.emplace_back("Depth (uncertainty)", formatValue(portrayed, component.uom));
					}
				}
				// The lookup entry the portrayed value falls in, "Deep water".
				for (const Fill& fill : fillsByFeature_)
				{
					if (fill.featureId != feature->id || fill.instruction.attributeCode != component.name)
					{
						continue;
					}
					if (const auto match = s100::coverage::lookupCoverageEntry(fill.instruction.lookup, portrayed);
						match && !fill.instruction.lookup[match->index].label.empty())
					{
						rows.emplace_back("Portrayal", fill.instruction.lookup[match->index].label);
					}
					break;
				}
			}
			if (rows.empty())
			{
				continue;
			}
			if (section.title.empty())
			{
				const auto named = featureNames_.find(feature->code);
				section.title = named != featureNames_.end() && !named->second.empty() ? named->second
					: !feature->label.empty() && feature->label != feature->code ? feature->label : spacedCode(feature->code);
			}
			if (const auto& time = feature->timeGroups[*timeIndex].time)
			{
				rows.insert(rows.begin(), { "Time", s100::coverage::formatWlaDateTime(*time) });
			}
			std::ranges::move(rows, std::back_inserter(section.rows));
		}
		if (section.rows.empty())
		{
			return std::nullopt;
		}
		return section;
	}

	std::vector<CoverageDataset::PickSection> CoverageDataset::PickAllAt(const s100::coverage::GeoPosition& position)
	{
		std::vector<PickSection> sections;
		for (CoverageDataset* dataset : registry())
		{
			if (dataset->m_pLayer && !dataset->m_pLayer->IsOn())
			{
				continue;
			}
			if (auto section = dataset->PickAt(position))
			{
				sections.push_back(std::move(*section));
			}
		}
		std::ranges::sort(sections, {}, &PickSection::dataset);
		return sections;
	}
}
