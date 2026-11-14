#pragma once

#include "S100SpatialObject.h"

#include "../Coverage/ArrowThinning.h"
#include "../Coverage/CoverageInstructions.h"
#include "../Coverage/CoverageModel.h"
#include "../Coverage/CoverageRaster.h"
#include "../Coverage/DepthLines.h"
#include "../Coverage/WaterLevelAdjustment.h"

#include <map>
#include <optional>
#include <set>
#include <memory>
#include <string>
#include <vector>

class FeatureCatalogue;
class PortrayalCatalogue;
class Scaler;
struct ID2D1Bitmap;
struct ID2D1StrokeStyle;

namespace GF
{
	class FeatureType;
}

namespace GISLibrary
{
	class D2D1Resources;

	// An S-100 Part 10c gridded dataset (S-102, S-111, ...) portrayed by its portrayal catalogue: the
	// catalogue's rules (Lua, or XSLT as S-111's) run over one feature per coverage instance
	// (primitive Coverage, no attributes), and each CoverageFill they emit is drawn as one image of
	// the grid coloured by its lookup table (Documents/TechDocs/2026-11-11-coverage-backport-plan.md,
	// CV-3). The values are read from the file when first drawn; the images are kept until the
	// colours change.
	//
	// A CoverageFill whose lookup entries carry symbols (S-111 surface current arrows) is drawn as
	// those symbols at the grid points, turned and scaled by the point's values. Zoomed out, only
	// every nth row and column is drawn so that the arrows do not run over each other (S-111
	// Edition 2.0.0 clause 9.3.2, Coverage/ArrowThinning.h).
	//
	// An S-102 dataset is drawn with its depths adjusted by the depth settings (S-98 Appendix D):
	// the uncertainty selector and water level adjustment by the S-104 datasets open at the time.
	// S-104 datasets serve as those water levels and are not drawn themselves (hidden, as in
	// opens100-web by default).
	class CoverageDataset final : public S100SpatialObject
	{
	public:
		CoverageDataset();
		~CoverageDataset() override;

		// Reads the dataset's metadata (no values) and gives its layer the dataset's extent.
		bool Open(const std::wstring& path) override;
		bool OpenMetadata(const std::wstring& /*path*/) override { return true; }

		// Runs the catalogue's rules with the viewer's depth settings (SafetyContour,
		// ShallowContour, DeepContour); false when there is nothing to portray with.
		bool Portray(FeatureCatalogue* fc, PortrayalCatalogue* pc);
		// Draws every CoverageFill into the device context, in the current palette, and over it the
		// S-98 Appendix D lines when EnhancedSafetyContour is on.
		void DrawCoverage(D2D1Resources* d2, HDC& hDC, Scaler* scaler);

		// The viewer's S-98 depth settings: the enhanced safety contour selector (D-1.4: the safety
		// contour drawn from the grid at the viewer's safety contour, with the S-102 coverage
		// boundary; on by default, as in opens100-web), the uncertainty selector and WLA.
		struct DepthSettings
		{
			bool enhancedSafetyContour = true;
			s100::coverage::DepthAdjustmentSettings adjustment;
		};
		[[nodiscard]] static const DepthSettings& GetDepthSettings() noexcept;
		// Every S-102 dataset is adjusted again (and redrawn) the next time it is drawn.
		static void SetDepthSettings(const DepthSettings& settings);

		// What WLA did when the S-102 datasets were last drawn, for the permanent indication.
		struct WlaStatus
		{
			std::size_t applied = 0;
			std::set<s100::coverage::WlaIssue> issues;
			std::optional<s100::coverage::VerticalDatum> s102Datum;
			std::optional<s100::coverage::VerticalDatum> s104Datum;
		};
		[[nodiscard]] static WlaStatus CurrentWlaStatus();
		// The first and last S-104 record times over the S-104 datasets open now.
		[[nodiscard]] static std::optional<std::pair<s100::coverage::TimePoint, s100::coverage::TimePoint>> WaterLevelTimeSpan();

		// How time-varying coverages (S-111) are shown.
		struct DisplaySettings
		{
			// The time whose record is drawn (S-111 Edition 2.0.0 clause 9.4: the closest record
			// before it, nothing outside the dataset's time span); the system clock when not set.
			// A dataset with no record at the system clock is then drawn at its first record, so
			// that data of another period shows when it is opened; the viewer says so on screen.
			std::optional<s100::coverage::TimePoint> time;
			// Whether the symbols of a grid are thinned as the chart is zoomed out (clause 9.3.2).
			bool thinSymbols = true;
		};
		[[nodiscard]] static const DisplaySettings& GetDisplaySettings() noexcept;
		static void SetDisplaySettings(const DisplaySettings& settings);
		// The first and last record times over the drawn datasets that vary with time (S-104, which
		// is not drawn, left out).
		[[nodiscard]] static std::optional<std::pair<s100::coverage::TimePoint, s100::coverage::TimePoint>> DisplayTimeSpan();
		// The record times of those datasets, in order and each once: the times there is something
		// to show at.
		[[nodiscard]] static std::vector<s100::coverage::TimePoint> DisplayRecordTimes();
		// Whether those datasets have a record to draw at the display time: how many there are,
		// how many of them are not drawn because the set time is outside their records, and how
		// many are drawn at their first record because the system clock is (the earliest of those
		// first records is firstRecordShown).
		struct DisplayTimeStatus
		{
			std::size_t datasets = 0;
			std::size_t withoutRecord = 0;
			std::size_t atFirstRecord = 0;
			std::optional<s100::coverage::TimePoint> firstRecordShown;
		};
		[[nodiscard]] static DisplayTimeStatus CurrentDisplayTimeStatus();

		// What the last DrawCoverage drew as symbols: the thinning factor (1: every grid point; 0
		// when no symbols were drawn) and how many symbols.
		struct SymbolStatus
		{
			int thinningFactor = 0;
			std::size_t drawn = 0;
		};
		[[nodiscard]] const SymbolStatus& LastSymbolStatus() const noexcept { return symbolStatus_; }

		// S-98 D-1: where S-102 data covers the water, the S-101 depth features (DepthArea,
		// DredgedArea, DepthContour) are suppressed while the enhanced safety contour is on. The
		// outlines of the S-102 cells with data, over every S-102 dataset alive now (in degrees);
		// empty when nothing is suppressed. The version changes whenever the outlines may have.
		[[nodiscard]] static std::uint64_t SuppressionVersion() noexcept;
		[[nodiscard]] static std::vector<std::vector<s100::coverage::GeoPosition>> SuppressionRings();

		// The pick report of the values under a point (opens100-web viewer-ol picking.ts): each
		// value of the drawn features as the file gives it, with its unit (S-98 14.1 rules 3 and
		// 4); where WLA applies, the water level and the adjusted depth as well (D-2 item 3b), or
		// the depth less uncertainty where only that applies; and the lookup entry the portrayed
		// value falls in.
		struct PickSection
		{
			std::string dataset;   // the file's name
			std::string product;   // "S-102"
			std::string title;     // the feature's name
			std::vector<std::pair<std::string, std::string>> rows;   // name, value
		};
		// Empty when the point has no value of this dataset's drawn features.
		[[nodiscard]] std::optional<PickSection> PickAt(const s100::coverage::GeoPosition& position);
		// Over every coverage dataset alive and drawn now, in the order of their names. S-104 is not
		// drawn and is reported through the S-102 water level.
		[[nodiscard]] static std::vector<PickSection> PickAllAt(const s100::coverage::GeoPosition& position);

		// The values drawn: the depth-adjusted coverage for S-102, the file's for the others.
		[[nodiscard]] const s100::coverage::CoverageSource* GetCoverage() const noexcept { return Source(); }
		// The file's values, unadjusted (for the pick report).
		[[nodiscard]] const s100::coverage::CoverageSource* GetRawCoverage() const noexcept { return raw_.get(); }
		[[nodiscard]] const std::vector<s100::coverage::CoverageInstruction>& GetInstructions() const noexcept { return fills_; }

		int GetFeatureCount() override { return static_cast<int>(features_.size()); }
		GF::FeatureType* GetFeatureTypeByIndex(int index) override;
		GF::FeatureType* GetFeatureType(std::string id) override;

	private:
		struct Fill
		{
			std::string featureId;
			s100::coverage::CoverageInstruction instruction;
		};
		struct Lines
		{
			std::string key;   // safety contour, revision of the values
			s100::coverage::DepthLines lines;
		};
		struct Image
		{
			std::string key;   // palette, time group, revision of the values
			std::unique_ptr<s100::coverage::CoverageRaster> raster;
			ID2D1Bitmap* bitmap = nullptr;
		};

		void ReleaseImages();
		// The catalogue's XSLT rules over the coverage features: the display list's coverage
		// instructions (S-111).
		void PortrayXslt(FeatureCatalogue* fc, PortrayalCatalogue* pc, const std::string& mainRule);
		void DrawSymbols(D2D1Resources* d2, Scaler* scaler, const s100::coverage::CoverageFeature& feature,
			const s100::coverage::CoverageInstruction& instruction, std::size_t component, std::size_t timeIndex);
		void DrawLines(D2D1Resources* d2, Scaler* scaler, const s100::coverage::CoverageFeature& feature,
			const s100::coverage::Values& depth, double fillValue);
		[[nodiscard]] const s100::coverage::CoverageSource* Source() const noexcept;
		// Adjusts the S-102 depths again when the settings or the S-104 datasets changed.
		void PrepareAdjustment();
		[[nodiscard]] ID2D1StrokeStyle* DashedStroke(D2D1Resources* d2, const s100::coverage::S98LineStyle& style, ID2D1StrokeStyle*& cache);
		[[nodiscard]] s100::coverage::Rgb ResolveColor(std::string_view token) const;

		std::shared_ptr<const s100::coverage::CoverageSource> raw_;
		std::shared_ptr<s100::coverage::WaterLevelAdjustedCoverage> adjusted_;   // S-102 only
		std::uint64_t adjustedFor_ = 0;   // the settings version adjusted_ was computed for
		std::vector<const s100::coverage::CoverageSource*> adjustedSources_;
		s100::coverage::WlaReport report_;
		std::optional<std::vector<s100::coverage::GridRing>> validRings_;   // S-102: computed when first asked for
		std::vector<std::unique_ptr<GF::FeatureType>> features_;
		std::vector<Fill> fillsByFeature_;
		std::vector<s100::coverage::CoverageInstruction> fills_;
		std::map<std::string, Image> images_;   // by feature ID
		std::map<std::string, Lines> lines_;    // by feature ID
		// The largest symbol scale over the values drawn, by feature ID; the key is the time group
		// and the revision of the values.
		std::map<std::string, std::pair<std::string, double>> symbolScales_;
		PortrayalCatalogue* pc_ = nullptr;   // the catalogue last portrayed with: its symbols
		SymbolStatus symbolStatus_;
		// The first record, when it is drawn in place of the record of the system clock: no time
		// is set and this dataset has records, none of them at the system clock.
		[[nodiscard]] std::optional<s100::coverage::TimePoint> FirstRecordInsteadOfNow() const;
		// The time whose records of this dataset are drawn.
		[[nodiscard]] s100::coverage::TimePoint DisplayTime() const;
		ID2D1StrokeStyle* s102BoundaryStroke_ = nullptr;
		ID2D1StrokeStyle* wlaBoundaryStroke_ = nullptr;

		// The feature catalogue's names, by feature code and by attribute code (the pick report).
		std::map<std::string, std::string> featureNames_;
		std::map<std::string, std::string> attributeNames_;

		// The catalogue's colour profile, token -> sRGB per palette name.
		std::map<std::string, std::map<std::string, s100::coverage::Rgb>> palettes_;
	};
}
