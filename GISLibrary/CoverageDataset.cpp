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
#include "../S100Engine/S100ColorProfile.h"
#include "../S100Engine/S100PCManager.h"
#include "../S100Engine/SVGReader.h"

#include <pugixml.hpp>

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

		CoverageDataset::DisplaySettings& displaySettings()
		{
			static CoverageDataset::DisplaySettings value;
			return value;
		}

		// The time whose records are drawn: the one set, or now.
		s100::coverage::TimePoint displayTime()
		{
			return displaySettings().time.value_or(
				std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
		}

		// An element name without its namespace prefix: catalogues differ in the prefixes they use.
		std::string_view localName(const pugi::xml_node& node)
		{
			const std::string_view name = node.name();
			const auto colon = name.rfind(':');
			return colon == std::string_view::npos ? name : name.substr(colon + 1);
		}

		pugi::xml_node childNamed(const pugi::xml_node& parent, std::string_view name)
		{
			for (const pugi::xml_node& child : parent.children())
			{
				if (localName(child) == name) return child;
			}
			return {};
		}

		std::optional<double> numberOf(const char* text)
		{
			if (!text || !*text)
			{
				return std::nullopt;
			}
			char* end = nullptr;
			const double value = std::strtod(text, &end);
			return end != text ? std::optional<double>(value) : std::nullopt;
		}

		// <symbol reference rotationCRS defaultScaleFactor rotation><rotationAttribute/><rotationFactor/>
		// <scaleAttribute/><scaleFactor/></symbol> of a coverage lookup (S-100 Part 9, 9-12.x).
		s100::coverage::CoverageSymbol symbolOf(const pugi::xml_node& node)
		{
			s100::coverage::CoverageSymbol symbol;
			symbol.reference = node.attribute("reference").as_string();
			symbol.rotationCRS = node.attribute("rotationCRS").as_string();
			symbol.rotation = node.attribute("rotation").as_double(0);
			symbol.defaultScaleFactor = node.attribute("defaultScaleFactor").as_double(1);
			if (const auto attribute = childNamed(node, "rotationAttribute"))
			{
				symbol.rotationAttribute = attribute.text().as_string();
				symbol.rotationFactor = numberOf(childNamed(node, "rotationFactor").text().as_string()).value_or(1);
			}
			if (const auto attribute = childNamed(node, "scaleAttribute"))
			{
				symbol.scaleAttribute = attribute.text().as_string();
				symbol.scaleFactor = numberOf(childNamed(node, "scaleFactor").text().as_string()).value_or(1);
			}
			return symbol;
		}

		// The coverage instructions of a display list (the output of a catalogue's XSLT rules), with
		// the feature each refers to.
		std::vector<std::pair<std::string, s100::coverage::CoverageInstruction>> coverageInstructionsOf(const std::string& displayList)
		{
			std::vector<std::pair<std::string, s100::coverage::CoverageInstruction>> result;
			pugi::xml_document document;
			if (!document.load_buffer(displayList.data(), displayList.size()))
			{
				return result;
			}
			for (const pugi::xml_node& node : document.document_element().children())
			{
				if (localName(node) != "coverageInstruction")
				{
					continue;
				}
				s100::coverage::CoverageInstruction base;
				for (const pugi::xml_node& child : node.children())
				{
					if (localName(child) == "viewingGroup") base.viewingGroups.push_back(child.text().as_int());
				}
				base.drawingPriority = childNamed(node, "drawingPriority").text().as_int();
				if (const auto plane = childNamed(node, "displayPlane")) base.displayPlane = plane.text().as_string();
				const std::string featureId = childNamed(node, "featureReference").text().as_string();
				for (const pugi::xml_node& fill : node.children())
				{
					if (localName(fill) != "coverageFill")
					{
						continue;
					}
					s100::coverage::CoverageInstruction instruction = base;
					instruction.attributeCode = childNamed(fill, "attributeCode").text().as_string();
					if (const auto uom = childNamed(fill, "uom")) instruction.uom = uom.text().as_string();
					for (const pugi::xml_node& lookup : fill.children())
					{
						if (localName(lookup) != "lookup")
						{
							continue;
						}
						const auto range = childNamed(lookup, "range");
						const auto closure = s100::coverage::intervalClosureFromString(range.attribute("closure").as_string());
						if (!closure)
						{
							continue;
						}
						s100::coverage::LookupEntry entry{ .label = childNamed(lookup, "label").text().as_string(),
							.lower = numberOf(range.attribute("lower").as_string()), .upper = numberOf(range.attribute("upper").as_string()),
							.closure = *closure };
						if (const auto color = childNamed(lookup, "color"))
						{
							entry.color = s100::coverage::CoverageColor{ .startToken = color.attribute("token").as_string(color.text().as_string()),
								.startTransparency = color.attribute("transparency").as_double(0) };
						}
						if (const auto symbol = childNamed(lookup, "symbol"))
						{
							entry.symbol = symbolOf(symbol);
						}
						instruction.lookup.push_back(std::move(entry));
					}
					if (!instruction.attributeCode.empty())
					{
						result.emplace_back(featureId, std::move(instruction));
					}
				}
			}
			return result;
		}

		std::string xmlEscaped(std::string_view text)
		{
			std::string out;
			for (const char c : text)
			{
				switch (c)
				{
				case '&': out += "&amp;"; break;
				case '<': out += "&lt;"; break;
				case '"': out += "&quot;"; break;
				default: out.push_back(c);
				}
			}
			return out;
		}

		bool hasSymbols(const s100::coverage::CoverageInstruction& instruction)
		{
			return std::ranges::any_of(instruction.lookup, [](const auto& entry) { return entry.symbol.has_value(); });
		}
	}

	const CoverageDataset::DisplaySettings& CoverageDataset::GetDisplaySettings() noexcept
	{
		return displaySettings();
	}

	void CoverageDataset::SetDisplaySettings(const DisplaySettings& value)
	{
		displaySettings() = value;
	}

	std::optional<std::pair<s100::coverage::TimePoint, s100::coverage::TimePoint>> CoverageDataset::DisplayTimeSpan()
	{
		const auto times = DisplayRecordTimes();
		return times.empty() ? std::nullopt : std::optional(std::pair{ times.front(), times.back() });
	}

	std::vector<s100::coverage::TimePoint> CoverageDataset::DisplayRecordTimes()
	{
		std::set<s100::coverage::TimePoint> times;
		for (CoverageDataset* dataset : registry())
		{
			if (!dataset->raw_ || dataset->fillsByFeature_.empty())
			{
				continue;
			}
			for (const auto& feature : dataset->raw_->dataset().features)
			{
				for (const auto& group : feature.timeGroups)
				{
					if (group.time) times.insert(*group.time);
				}
			}
		}
		return { times.begin(), times.end() };
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
		symbolScales_.clear();
		pc_ = pc;
		const bool xslt = pc && pc->GetRuleFileFormat() == Portrayal::FileFormat::XSLT;
		if (!raw_ || !fc || !pc || (pc->GetRuleFileFormat() != Portrayal::FileFormat::LUA && !xslt) || !pc->GetMainRuleFile())
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
		if (xslt)
		{
			PortrayXslt(fc, pc, utf8(mainRule));
			return !fills_.empty();
		}
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

	void CoverageDataset::PortrayXslt(FeatureCatalogue* /*fc*/, PortrayalCatalogue* pc, const std::string& mainRule)
	{
		// The rules see each coverage instance as one feature of primitive Coverage (S-100 Part 9,
		// 9-7.2): <Dataset><Features><SurfaceCurrent id="..." primitive="Coverage"/></Features></Dataset>.
		std::string input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Dataset><Features>";
		for (const auto& feature : raw_->dataset().features)
		{
			if (feature.code.empty() || !(std::isalpha(static_cast<unsigned char>(feature.code.front())) || feature.code.front() == '_'))
			{
				continue;
			}
			input += std::format("<{} id=\"{}\" primitive=\"Coverage\"/>", feature.code, xmlEscaped(feature.id));
		}
		input += "</Features></Dataset>";
		for (auto& [featureId, instruction] : coverageInstructionsOf(ProcessS101::ProcessS100_XSLT(input, mainRule, pc)))
		{
			if (!raw_->dataset().feature(featureId))
			{
				continue;
			}
			fills_.push_back(instruction);
			fillsByFeature_.push_back({ featureId, std::move(instruction) });
		}
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
		const auto now = displayTime();
		symbolStatus_ = {};
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
			if (hasSymbols(fill.instruction))
			{
				DrawSymbols(d2, scaler, *feature, fill.instruction, *component, *timeIndex);
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
		rt->SetTransform(D2D1::Matrix3x2F::Identity());
		if (rt->EndDraw() == static_cast<HRESULT>(D2DERR_RECREATE_TARGET))
		{
			ReleaseImages();
		}
	}

	void CoverageDataset::DrawSymbols(D2D1Resources* d2, Scaler* scaler, const s100::coverage::CoverageFeature& feature,
		const s100::coverage::CoverageInstruction& instruction, std::size_t component, std::size_t timeIndex)
	{
		namespace cov = s100::coverage;
		auto* manager = pc_ ? pc_->GetS100PCManager() : nullptr;
		auto* profile = manager ? manager->GetS100ColorProfile() : nullptr;
		auto* rt = d2->RenderTarget();
		auto* brush = d2->SolidColorBrush();
		if (!manager || !profile || !rt || !brush)
		{
			return;
		}
		const cov::CoverageSource* source = Source();
		const auto values = source->readValues(feature.id, { .timeIndex = timeIndex, .component = component });
		if (!values || values->size() < feature.grid.nodeCount())
		{
			return;
		}
		const double fillValue = feature.components[component].fillValue;

		// The symbols of the entries, with the values they are turned and scaled by.
		struct Drawn
		{
			const cov::CoverageSymbol* symbol = nullptr;
			SVGReader* svg = nullptr;
			cov::Values rotation;
			double rotationFill = 0;
			cov::Values scale;
			double scaleFill = 0;
		};
		const auto valuesOf = [&](const std::optional<std::string>& attribute, cov::Values& out, double& fill) {
			if (!attribute)
			{
				return true;
			}
			const auto index = feature.componentIndex(*attribute);
			if (!index)
			{
				return false;
			}
			out = *index == component ? values : source->readValues(feature.id, { .timeIndex = timeIndex, .component = *index });
			fill = feature.components[*index].fillValue;
			return out && out->size() >= feature.grid.nodeCount();
		};
		std::vector<Drawn> drawn(instruction.lookup.size());
		float symbolLength = 0;   // in the symbols' own unit, millimetres
		for (std::size_t i = 0; i < instruction.lookup.size(); ++i)
		{
			const auto& entry = instruction.lookup[i];
			if (!entry.symbol)
			{
				continue;
			}
			SVGReader* svg = manager->s100SymbolManager.GetSVG(entry.symbol->reference);
			if (!svg)
			{
				continue;
			}
			// The catalogue's symbol geometry, when the viewer has not prepared it yet
			// (PortrayalRender::Prepare).
			if (svg->geometry.empty() && !svg->figures.empty() && d2->pD2Factory)
			{
				manager->CreateSVGGeometry(d2->pD2Factory);
			}
			Drawn item{ .symbol = &*entry.symbol, .svg = svg };
			if (!valuesOf(entry.symbol->rotationAttribute, item.rotation, item.rotationFill)
				|| !valuesOf(entry.symbol->scaleAttribute, item.scale, item.scaleFill))
			{
				continue;
			}
			symbolLength = (std::max)(symbolLength, (std::max)(svg->bounding_width, svg->bounding_height));
			drawn[i] = std::move(item);
		}
		if (!(symbolLength > 0))
		{
			symbolLength = static_cast<float>(cov::ArrowScale{}.symbolLength);
		}

		// The largest scale over the field at this time: the longest symbol (L_smax of S-111 9.3.2).
		const std::string key = std::format("{}|{}", timeIndex, source->revision());
		auto& maxScale = symbolScales_[feature.id + "|" + instruction.attributeCode];
		if (maxScale.first != key)
		{
			double largest = 0;
			const std::size_t nodes = feature.grid.nodeCount();
			for (std::size_t n = 0; n < nodes; ++n)
			{
				const float value = (*values)[n];
				if (cov::isNoData(value, fillValue))
				{
					continue;
				}
				const auto match = cov::lookupCoverageEntry(instruction.lookup, value);
				if (!match || !drawn[match->index].symbol)
				{
					continue;
				}
				const Drawn& item = drawn[match->index];
				const std::optional<double> scaleValue = item.scale && !cov::isNoData((*item.scale)[n], item.scaleFill)
					? std::optional<double>((*item.scale)[n]) : std::nullopt;
				largest = (std::max)(largest, cov::symbolScale(*item.symbol, scaleValue));
			}
			maxScale = { key, largest };
		}
		if (!(maxScale.second > 0))
		{
			return;   // no value at this time has a symbol
		}

		// Millimetres at 96 device pixels per inch, as the lines over S-102 are drawn.
		constexpr float pixelsPerMillimetre = 96.0f / 25.4f;
		const cov::ToScreen toScreen = [scaler](const cov::GeoPosition& position) {
			double x = position.lon;
			double y = position.lat;
			projection(x, y);
			float sx = 0;
			float sy = 0;
			scaler->WorldToDevice_F(x, y, &sx, &sy);
			return cov::ScreenPoint{ sx, sy };
		};
		MBR map = scaler->GetMap();
		inverseProjection(map);
		const cov::GeoBounds view{ .minLon = map.GetXMin(), .minLat = map.GetYMin(), .maxLon = map.GetXMax(), .maxLat = map.GetYMax() };
		const double longest = static_cast<double>(symbolLength) * maxScale.second * pixelsPerMillimetre;
		const cov::ThinnedNodes nodes = displaySettings().thinSymbols
			? cov::thinArrows(feature.grid, toScreen, longest, view)
			: cov::thinnedNodes(feature.grid, 1, view);
		symbolStatus_.thinningFactor = nodes.factor;

		const RECT screen = D2DView::ScreenRECT(*scaler);
		const float margin = static_cast<float>(longest);
		for (int j = 0; j < nodes.rows; ++j)
		{
			for (int i = 0; i < nodes.cols; ++i)
			{
				const int col = nodes.col(i);
				const int row = nodes.row(j);
				const std::size_t n = static_cast<std::size_t>(row) * static_cast<std::size_t>(feature.grid.cols) + static_cast<std::size_t>(col);
				const float value = (*values)[n];
				if (cov::isNoData(value, fillValue))
				{
					continue;
				}
				const auto match = cov::lookupCoverageEntry(instruction.lookup, value);
				if (!match || !drawn[match->index].symbol)
				{
					continue;
				}
				const Drawn& item = drawn[match->index];
				// A point without a direction has no arrow.
				if (item.rotation && cov::isNoData((*item.rotation)[n], item.rotationFill))
				{
					continue;
				}
				const std::optional<double> rotationValue = item.rotation ? std::optional<double>((*item.rotation)[n]) : std::nullopt;
				const std::optional<double> scaleValue = item.scale && !cov::isNoData((*item.scale)[n], item.scaleFill)
					? std::optional<double>((*item.scale)[n]) : std::nullopt;
				const cov::ScreenPoint at = toScreen(cov::gridPosition(feature.grid, col, row));
				if (at.x < screen.left - margin || at.x > screen.right + margin || at.y < screen.top - margin || at.y > screen.bottom + margin)
				{
					continue;
				}
				const float scale = static_cast<float>(cov::symbolScale(*item.symbol, scaleValue)) * pixelsPerMillimetre;
				const float rotation = static_cast<float>(cov::symbolRotation(*item.symbol, rotationValue));
				// The symbol's millimetres to pixels, turned clockwise from north (up), at the point.
				rt->SetTransform(D2D1::Matrix3x2F::Scale(scale, scale) * D2D1::Matrix3x2F::Rotation(rotation)
					* D2D1::Matrix3x2F::Translation(static_cast<float>(at.x), static_cast<float>(at.y)));
				for (const SVGGeometry& shape : item.svg->geometry)
				{
					if (!shape.pGeometry)
					{
						continue;
					}
					brush->SetOpacity(shape.alpha);
					if (shape.bFill)
					{
						brush->SetColor(profile->GetColor(shape.fillColorName));
						rt->FillGeometry(shape.pGeometry, brush);
					}
					if (shape.bStoke)
					{
						brush->SetColor(profile->GetColor(shape.strokeColorName));
						rt->DrawGeometry(shape.pGeometry, brush, shape.width, d2->SolidStrokeStyle());
					}
				}
				++symbolStatus_.drawn;
			}
		}
		brush->SetOpacity(1.0f);
		rt->SetTransform(D2D1::Matrix3x2F::Identity());
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
		const auto now = displayTime();
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
