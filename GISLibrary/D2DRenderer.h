#pragma once
#include "IRenderer.h"

class PortrayalCatalogue;

namespace GISLibrary
{
	class D2D1Resources;

	// IRenderer on the Direct2D render target of the viewer (the transitional backend,
	// DIRECTIVES 24). It draws with the shared brush and stroke styles of D2D1Resources, as the
	// code it replaces did.
	class D2DRenderer : public s100::render::IRenderer
	{
	public:
		// The catalogue is the one whose symbols are drawn.
		D2DRenderer(D2D1Resources* d2, PortrayalCatalogue* pc);

		// False when the Direct2D resources are not there (nothing can be drawn).
		bool Ready() const;

		void SetTransform(const s100::render::Transform& transform) override;
		void StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override;
		void DrawSymbol(const std::string& reference, s100::render::PointF at, float rotationDegrees, float scale) override;

	private:
		D2D1Resources* d2;
		PortrayalCatalogue* pc;
	};
}
