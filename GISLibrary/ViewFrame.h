#pragma once
// What a refresh of a view is drawn on (Documents/TechDocs/2026-11-24-r8-view-on-every-platform.md).
//
// The layers of a view - the background map, the charts, the grids, the overlay of an exchange
// catalogue - draw on a frame and do not know what is behind it: a window, a picture in memory
// or a file. A layer opens the frame over the view (Begin), draws on the renderer the frame
// gives it or has a drawing priority of S-100 instructions painted, and closes it (End). What a
// layer leaves set on the renderer (a clip, a transform) ends with its End.
//
// The frame of the Skia backend is SkiaViewFrame.

class DrawingSet;
class PortrayalCatalogue;
class Scaler;

namespace s100::render
{
	class IRenderer;
}

namespace GISLibrary
{
	class ViewFrame
	{
	public:
		virtual ~ViewFrame() = default;

		// Opens the frame over the view of a scaler. False when there is nothing to draw on;
		// End is not called then.
		virtual bool Begin(const Scaler& scaler) = 0;

		// The renderer for what is drawn with a catalogue (none: no symbols, no colour tokens).
		// Null outside Begin and End.
		virtual s100::render::IRenderer* Renderer(PortrayalCatalogue* catalogue) = 0;

		// One drawing priority of a drawing set, with the colours and symbols of a catalogue.
		virtual void Paint(int priority, DrawingSet& drawingSet, PortrayalCatalogue* catalogue, Scaler& scaler, bool texts) = 0;

		// Closes what Begin opened.
		virtual void End() = 0;

	protected:
		ViewFrame() = default;
		ViewFrame(const ViewFrame&) = default;
		ViewFrame& operator=(const ViewFrame&) = default;
	};
}
