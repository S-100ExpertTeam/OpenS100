#pragma once
// The frame of a view on Skia (Documents/TechDocs/2026-11-24-r8-view-on-every-platform.md).
//
// What the layers of a view draw is written down (a recording) and rasterised on pixels of the
// view's size when the frame is shown. The pixels are transparent where nothing was drawn.
//
// With no window: Draw a view, then WritePng or Pixels. With one: the application gives a
// target (FrameTarget) that puts the pixels on its window, over what is already there. The frame
// names no type of an operating system.
#include "DrawTypes.h"
#include "ViewFrame.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

class CGISLibraryApp;

namespace GISLibrary
{
	// The pixels of a frame, transparent where nothing was drawn: a byte a component, blue, green,
	// red, alpha in memory, the colours multiplied by alpha, the top row first. They are the
	// frame's and hold until it begins another.
	struct FramePixels
	{
		const std::uint8_t* data = nullptr;   // null when there is no frame to show
		int width = 0;
		int height = 0;
		std::size_t rowBytes = 0;
		// Where the view is on its device (the left and top of the screen rectangle of the scaler).
		int left = 0;
		int top = 0;
	};

	// Where an application shows a frame: a window, a device context, a texture.
	class FrameTarget
	{
	public:
		virtual ~FrameTarget() = default;
		// Lays the pixels over what the target shows. Called only for a frame that drew something.
		virtual void Show(const FramePixels& pixels) = 0;
	};

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
		// drawing, and that drawing is rasterised (and shown on the target) once, when the
		// scene closes: the picture is the same, and the pixels are
		// cleared and drawn once a refresh instead of once a layer. Scenes nest; the outermost
		// counts.
		class Scene
		{
		public:
			// Ends on the pixels of the frame.
			explicit Scene(SkiaViewFrame& frame);
			// Ends on a target, which outlives the scene.
			Scene(SkiaViewFrame& frame, FrameTarget* target);
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

		// Ends the frame on a target: what was painted is shown on it. In a scene on that target
		// this is put off until the scene closes.
		void End(FrameTarget* target);

		// Font files to write texts in, the display font first. With none (the default) the
		// font the viewer is set to (ENCCommon::DISPLAY_FONT_NAME) is asked of the system, which
		// only Windows answers.
		void SetFontFiles(std::vector<std::filesystem::path> files);

		// The pixels of the last frame rasterised; no data when it drew nothing.
		FramePixels Pixels() const;

		// The pixels of the last frame shown over a background colour, as a PNG file.
		bool WritePng(const std::filesystem::path& path, const ColorF& background) const;

		// Forgets what is kept for the catalogues (when the files of one change). What is kept
		// for a catalogue that is unloaded is forgotten by itself.
		void ForgetCatalogues();

	private:
		// Rasterises what is recorded and shows it on a target (none: the pixels only).
		void Show(FrameTarget* target);

		struct State;
		std::unique_ptr<State> state;
	};
}
