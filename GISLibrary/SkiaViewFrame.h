#pragma once
// The viewer's frames of S-100 drawing instructions on Skia
// (Documents/TechDocs/2026-11-24-r7-viewer-on-skia-design.md, 5).
//
// What Skia draws is a layer: a transparent surface the size of the view, painted by the shared
// painter on a SkiaRenderer and blended onto the device context the rest of the frame is drawn
// on. So what is still drawn by GDI (the background map, some overlays) keeps its place in the
// order.
//
// Windows only, like the device context it ends on; it goes when the whole frame is drawn on
// one surface.
#include "DeviceTypes.h"

#include <memory>

namespace s100::render
{
	class IRenderer;
}

class DrawingSet;
class PortrayalCatalogue;
class Scaler;

namespace GISLibrary
{
	class SkiaViewFrame final
	{
	public:
		// The frame of the process: the pictures of a catalogue's symbols are kept across frames.
		static SkiaViewFrame& Shared();

		SkiaViewFrame();
		~SkiaViewFrame();
		SkiaViewFrame(const SkiaViewFrame&) = delete;
		SkiaViewFrame& operator=(const SkiaViewFrame&) = delete;

		// A transparent surface the size of the view. False when there is none to draw on.
		bool Begin(const Scaler& scaler);

		// A refresh of the view draws many frames, one a layer or a group of layers, each laid over
		// the ones before on the same device context. While a scene is open the frames ended on
		// its device context are kept as one drawing, and that drawing is rasterised and blended
		// once, when the scene closes: the picture is the same, and the surface is cleared, drawn
		// and blended once a refresh instead of once a layer. Scenes nest; the outermost counts.
		class Scene
		{
		public:
			Scene(SkiaViewFrame& frame, HDC target);
			~Scene();
			Scene(const Scene&) = delete;
			Scene& operator=(const Scene&) = delete;

		private:
			SkiaViewFrame& frame;
		};

		// One drawing priority of a drawing set, with the colours and symbols of a catalogue.
		void Paint(int priority, DrawingSet& drawingSet, PortrayalCatalogue* catalogue, Scaler& scaler, bool texts);

		// The renderer of the frame in hand for what is drawn with a catalogue (none: no symbols).
		// Null outside a frame.
		s100::render::IRenderer* Renderer(PortrayalCatalogue* catalogue);

		// Blends what was painted onto the device context, at the place of the view. In a scene
		// on that device context this is put off until the scene closes.
		void End(HDC target);

		// Forgets what is kept for the catalogues (when one is unloaded or its files change).
		void ForgetCatalogues();

	private:
		// Rasterises what is recorded and blends it onto a device context.
		void Show(HDC target);

		struct State;
		std::unique_ptr<State> state;
	};
}
