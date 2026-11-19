#pragma once
// What of a display list a view shows and the order it is drawn in, with no backend in it.
//
// A frame of a dataset is three steps:
//   CollectInstructions   the instructions the view shows, sorted into a DrawingSet by drawing
//                         priority and kind
//   SuppressSharedCurves  a curve two line instructions share is drawn once, by the one of the
//                         higher priority
//   PaintPriority         one drawing priority after another, lowest first
// The viewer (LayerManager) and a backend with no viewer go through the same three.
#include <functional>
#include <set>
#include <string>

class DrawingSet;
class Scaler;
class SENC_DisplayList;
class SENC_Instruction;
class SENC_InstructionVisitor;

namespace s100::render
{
	// Whether the features of a type are shown (the user can turn a feature type off).
	using FeatureFilter = std::function<bool(const std::wstring& featureCode)>;

	// Adds the instructions of a display list that are shown at a scale in the view of the
	// scaler to the drawing set, and the drawing priorities that have any to `priorities`.
	// Which instructions those are is the display list's own rule (viewing group, scale
	// minimum and maximum, the extent of the feature); the filter drops feature types.
	void CollectInstructions(SENC_DisplayList& displayList, int scale, Scaler& scaler, const FeatureFilter& shown,
		DrawingSet& drawingSet, std::set<int>& priorities);

	// Marks the curves that are drawn by more than one line instruction of the set, so that
	// only the first one to come, from the highest priority down, draws them (S-101: the edge
	// two areas share is one line). A masked curve is left alone.
	void SuppressSharedCurves(const std::set<int>& priorities, DrawingSet& drawingSet);

	// What a display does around the groups of one drawing priority: a transform the backend
	// counts on, a mask over some of the instructions. Nothing by default.
	class DrawingStages
	{
	public:
		virtual ~DrawingStages() = default;

		// Areas and lines are drawn in the coordinates of the dataset.
		virtual void BeginAreasAndLines() {}
		virtual void BeforeArea(SENC_Instruction&) {}
		virtual void BeforeLine(SENC_Instruction&) {}
		virtual void EndAreasAndLines() {}

		// Texts are drawn last, in device pixels. `any`: the priority has a text.
		virtual void BeginTexts(bool /*any*/) {}
		virtual void BeforeText(SENC_Instruction&) {}
		virtual void EndTexts() {}
	};

	// Draws the instructions of one drawing priority of a set in the order of their kinds:
	// areas, lines, augmented rays, augmented paths, points and, if texts are shown, texts.
	void PaintPriority(int priority, DrawingSet& drawingSet, SENC_InstructionVisitor& painter, bool texts,
		DrawingStages* stages = nullptr);
}
