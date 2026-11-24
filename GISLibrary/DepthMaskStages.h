#pragma once
// S-98 Edition 3.0.0 D-1 on any backend: with the enhanced safety contour on, the S-101 depth
// features (DepthArea, DredgedArea, DepthContour) are not drawn where S-102 data covers the
// water, so the two depth portrayals do not overlap.
//
// Their instructions are drawn through a clip: the view less the outlines of the S-102 data.
// The clip is pushed at the start of a run of such instructions in a list and popped at its
// end, so the order of the list is kept.
#include "DrawingOrder.h"
#include "IRenderer.h"

#include <string_view>
#include <vector>

class Scaler;
class SENC_Instruction;

namespace s100::render
{
	// A point in the coordinates datasets are kept in (projected).
	struct WorldPoint
	{
		double x = 0.0;
		double y = 0.0;
	};

	// The outlines of the data that hides the depth features, each a closed ring.
	using WorldRings = std::vector<std::vector<WorldPoint>>;

	// DepthArea, DredgedArea, DepthContour.
	[[nodiscard]] bool IsDepthFeatureUnderCoverage(std::string_view featureCode) noexcept;
	[[nodiscard]] bool IsDepthFeatureUnderCoverage(const SENC_Instruction& instruction);

	class DepthMaskStages final : public DrawingStages
	{
	public:
		// The rings outlive the stages. With no ring nothing is clipped.
		DepthMaskStages(IRenderer& renderer, Scaler& scaler, const WorldRings& rings);
		~DepthMaskStages() override;
		DepthMaskStages(const DepthMaskStages&) = delete;
		DepthMaskStages& operator=(const DepthMaskStages&) = delete;

		void BeforeArea(SENC_Instruction& instruction) override;
		void BeforeLine(SENC_Instruction& instruction) override;
		void EndAreasAndLines() override;
		void BeforeText(SENC_Instruction& instruction) override;
		void EndTexts() override;

	private:
		void Update(const SENC_Instruction& instruction);
		void Close();

		IRenderer& renderer;
		Scaler& scaler;
		const WorldRings& rings;
		Path mask;          // the view less the rings, in device pixels; made at the first run
		bool made = false;
		bool open = false;
	};
}
