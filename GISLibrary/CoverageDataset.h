#pragma once

#include "S100SpatialObject.h"

#include "../Coverage/CoverageInstructions.h"
#include "../Coverage/CoverageModel.h"
#include "../Coverage/CoverageRaster.h"
#include "../Coverage/DepthLines.h"

#include <map>
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

	// An S-100 Part 10c gridded dataset (S-102, ...) portrayed by its portrayal catalogue: the
	// catalogue's Lua rules run over one feature per coverage instance (primitive Coverage, no
	// attributes), and each CoverageFill they emit is drawn as one image of the grid coloured by
	// its lookup table (Documents/TechDocs/2026-11-11-coverage-backport-plan.md, CV-3). The values
	// are read from the file when first drawn; the images are kept until the colours change.
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

		// The viewer's "Enhanced safety contour" selector (S-98 D-1.4): the safety contour drawn
		// from the grid at the viewer's safety contour, with the S-102 coverage boundary. On by
		// default, as in opens100-web.
		static inline bool EnhancedSafetyContour = true;

		[[nodiscard]] const s100::coverage::CoverageSource* GetCoverage() const noexcept { return source_.get(); }
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
		void DrawLines(D2D1Resources* d2, Scaler* scaler, const s100::coverage::CoverageFeature& feature,
			const s100::coverage::Values& depth, double fillValue);
		[[nodiscard]] s100::coverage::Rgb ResolveColor(std::string_view token) const;

		std::unique_ptr<s100::coverage::CoverageSource> source_;
		std::vector<std::unique_ptr<GF::FeatureType>> features_;
		std::vector<Fill> fillsByFeature_;
		std::vector<s100::coverage::CoverageInstruction> fills_;
		std::map<std::string, Image> images_;   // by feature ID
		std::map<std::string, Lines> lines_;    // by feature ID
		ID2D1StrokeStyle* s102BoundaryStroke_ = nullptr;

		// The catalogue's colour profile, token -> sRGB per palette name.
		std::map<std::string, std::map<std::string, s100::coverage::Rgb>> palettes_;
	};
}
