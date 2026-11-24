#pragma once
// The frame of a view on Skia (Documents/TechDocs/2026-11-24-r8-view-on-every-platform.md).
//
// What the layers of a view draw is written down (a recording) and rasterised on pixels of the
// view's size when the frame is shown. The pixels are transparent where nothing was drawn.
//
// With no window: Draw a view, then WritePng. On Windows the pixels are blended onto a device
// context, over what is already there (CGISLibraryApp::Draw on a device context, or End on one).
#include "DeviceTypes.h"
#include "DrawTypes.h"
#include "ViewFrame.h"

#include <filesystem>
#include <memory>
#include <vector>

class CGISLibraryApp;

namespace GISLibrary
{
	class SkiaViewFrame final : public ViewFrame
	{
	public:
		// The frame of the process: the pictures of a catalogue's symbols are kept across frames.
		static SkiaViewFrame& Shared();

		SkiaViewFrame();
		~SkiaViewFrame() override;
		SkiaViewFrame(const SkiaViewFrame&) = delete;
		SkiaViewFrame& operator=(const SkiaViewFrame&) = delete;

		// A refresh of the view draws many frames, one a layer or a group of layers, each laid over
		// the ones before. While a scene is open the frames ended on its target are kept as one
		// drawing, and that drawing is rasterised (and on Windows blended onto the device
		// context) once, when the scene closes: the picture is the same, and the pixels are
		// cleared and drawn once a refresh instead of once a layer. Scenes nest; the outermost
		// counts.
		class Scene
		{
		public:
			// Ends on the pixels of the frame.
			explicit Scene(SkiaViewFrame& frame);
			// Ends on a device context (Windows).
			Scene(SkiaViewFrame& frame, HDC target);
			~Scene();
			Scene(const Scene&) = delete;
			Scene& operator=(const Scene&) = delete;

		private:
			SkiaViewFrame& frame;
		};

		// A refresh of a view as one scene, on the pixels of the frame.
		void Draw(CGISLibraryApp& view, int offset = 0);

		// A transparent surface the size of the view. False when there is none to draw on.
		bool Begin(const Scaler& scaler) override;

		// One drawing priority of a drawing set, with the colours and symbols of a catalogue.
		void Paint(int priority, DrawingSet& drawingSet, PortrayalCatalogue* catalogue, Scaler& scaler, bool texts) override;

		// The renderer of the frame in hand for what is drawn with a catalogue (none: no symbols).
		// Null outside a frame.
		s100::render::IRenderer* Renderer(PortrayalCatalogue* catalogue) override;

		// Ends the frame on the target of the scene in hand; with no scene open it is rasterised
		// on the pixels at once.
		void End() override;

		// Ends the frame on a device context (Windows): what was painted is blended onto it at
		// the place of the view. In a scene on that device context this is put off until the
		// scene closes.
		void End(HDC target);

		// Font files to write texts in, the display font first. With none (the default) the
		// font the viewer is set to (ENCCommon::DISPLAY_FONT_NAME) is asked of the system, which
		// only Windows answers.
		void SetFontFiles(std::vector<std::filesystem::path> files);

		// The pixels of the last frame shown over a background colour, as a PNG file.
		bool WritePng(const std::filesystem::path& path, const ColorF& background) const;

		// Forgets what is kept for the catalogues (when one is unloaded or its files change).
		void ForgetCatalogues();

	private:
		// Rasterises what is recorded and, on Windows, blends it onto a device context.
		void Show(HDC target);

		struct State;
		std::unique_ptr<State> state;
	};
}
