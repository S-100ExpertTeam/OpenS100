#pragma once

#include <string_view>

struct ID2D1Factory1;
struct ID2D1Geometry;
struct ID2D1RenderTarget;
struct ID2D1Layer;
class Scaler;
class SENC_Instruction;

namespace GISLibrary
{
	// S-98 Edition 3.0.0 D-1: with the enhanced safety contour on, the S-101 depth features are not
	// drawn where S-102 data covers the water, so the two depth portrayals do not overlap
	// (opens100-web viewer-ol depth-settings.ts). Their instructions are drawn through a mask of
	// the view less the outlines of the S-102 cells with data.
	namespace DepthSuppression
	{
		// DepthArea, DredgedArea, DepthContour.
		[[nodiscard]] bool IsSuppressedFeature(std::string_view featureCode) noexcept;

		// The S-102 data areas in world coordinates as drawn (projected, y negated), from
		// CoverageDataset::SuppressionRings, built again only when they change; null when nothing
		// is suppressed. Owned here.
		[[nodiscard]] ID2D1Geometry* DataAreas(ID2D1Factory1* factory);

		// Draws the suppressed instructions of a list through the mask: Update before each
		// instruction pushes the masked layer at the start of a run of them and pops it at its
		// end, so the order of the list is kept. The render target's transform must not change
		// while a run is open.
		class Runs final
		{
		public:
			// `worldTransform`: the target's transform is the scaler's world-to-device matrix (area
			// and line instructions); otherwise it is the identity (text in device pixels). Direct2D
			// applies the target's transform to a layer's mask after the mask's own transform.
			Runs(ID2D1RenderTarget* target, ID2D1Factory1* factory, const Scaler& scaler, bool worldTransform);
			Runs(const Runs&) = delete;
			Runs& operator=(const Runs&) = delete;
			~Runs();

			void Update(const SENC_Instruction* instruction);

		private:
			void Close();

			ID2D1RenderTarget* target_;
			ID2D1Geometry* mask_ = nullptr;   // the view less the data areas; owned
			ID2D1Layer* layer_ = nullptr;
			const Scaler* scaler_;
			bool worldTransform_;
			bool open_ = false;
		};
	}
}
