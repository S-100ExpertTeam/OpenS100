#pragma once
#include "IRenderer.h"
#include "SENC_InstructionVisitor.h"

#include <functional>
#include <list>
#include <string>
#include <vector>

class Scaler;
class SCurve;
class SSurface;
class SENC_LineStyleBase;

namespace s100::render
{
	// A colour token of the portrayal catalogue in the palette in use.
	using ColorLookup = std::function<ColorF(const std::wstring& token)>;

	// Draws drawing instructions on an IRenderer: the S-100 side of drawing, with no backend in
	// it. It works out where things go on the screen (projection, rotation of the view, north
	// at the point) and how they are stroked, and hands the backend paths.
	//
	// It draws every kind of instruction; the Direct2D painter of the viewer forwards all of
	// them here.
	//
	// A line or an area is drawn in the coordinates of the dataset under the transform of the
	// view, so the path of a curve or a surface is built once and kept by the backend. That
	// transform is left set after a line or an area; every other kind sets its own.
	//
	// With the chart zoomed out, most vertices of a coastline fall within a pixel of the one
	// before. A curve or a surface of many vertices is then drawn from a second kept path, in
	// which the vertices are gathered on a grid a quarter of a pixel wide at most and those that
	// fall together are one. The grid is the same for every geometry, so two surfaces that
	// share an edge still meet along it. That path is made again when the zoom has doubled or
	// halved.
	class DisplayListPainter : public SENC_InstructionVisitor, private SENC_LineStyleVisitor
	{
	public:
		DisplayListPainter(IRenderer& renderer, Scaler& scaler, ColorLookup color);

		// Whether a curve or a surface of many vertices is drawn, zoomed out, from the path with
		// the vertices gathered (see above). Off, every geometry is drawn whole at every zoom.
		inline static bool gatherVertices = true;

		void Visit(SENC_PointInstruction& instruction) override;
		void Visit(SENC_LineInstruction& instruction) override;
		void Visit(SENC_AreaInstruction& instruction) override;
		void Visit(SENC_TextInstruction& instruction) override;
		void Visit(SENC_AugmentedRay& instruction) override;
		void Visit(SENC_AugmentedPath& instruction) override;

	private:
		void Visit(SENC_LineStyle& lineStyle) override;
		void Visit(SENC_LineStyleReference& lineStyle) override;
		void Visit(SENC_CompositeLineStyle& lineStyle) override;

		// Draws one curve with a line style, whatever kind the style is.
		void DrawLineStyle(SENC_LineStyleBase* lineStyle, SCurve* curve);
		// Strokes the curve being styled: its kept path under the transform of the view, or,
		// zoomed far in, a path in device pixels.
		void StrokeCurve(const ColorF& penColor, const Stroke& stroke);
		// The curve being styled in device pixels, turned with the view if asked.
		void CurveOnDevice(bool turnWithView);
		// Fills the surface: its kept path under the transform of the view, or, zoomed far in, a
		// path in device pixels under the given transform.
		void FillSurface(SSurface* surface, const Fill& fill, const Transform& zoomedIn);
		// The dataset (x, -y) to device pixels, as Scaler::WorldToDevice has it.
		const Transform& ViewTransform();
		// How far the view is zoomed out, in doublings of the size of a pixel in the units of
		// the dataset.
		int ZoomLevel();

		IRenderer& renderer;
		Scaler& scaler;
		ColorLookup color;
		// Where a point or text instruction is drawn; kept to reuse its storage.
		std::vector<PointF> points;
		// The curve the line style being visited is drawn along, and its points on the device.
		SCurve* styleCurve = nullptr;
		std::vector<PointF> devicePoints;
		Transform viewTransform;
		bool viewTransformMade = false;
		int zoomLevel = 0;
		bool zoomLevelKnown = false;
	};
}
