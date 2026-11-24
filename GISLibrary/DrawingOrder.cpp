#include "stdafx.h"
#include "DrawingOrder.h"

#include "DrawingSet.h"
#include "R_FeatureRecord.h"
#include "SCompositeCurve.h"
#include "SCurve.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_InstructionVisitor.h"
#include "SENC_LineInstruction.h"
#include "SENC_SpatialReference.h"
#include "SGeometry.h"
#include "SSurface.h"

#include <list>
#include <unordered_set>
#include <vector>

namespace
{
	// The kinds of instruction a drawing set holds, by the type number of SENC_Instruction, and
	// the list of the set each goes to.
	struct Kind
	{
		int type;
		InstructionList* (DrawingSet::*list)(int);
	};

	const Kind kinds[] = {
		{ 7, &DrawingSet::GetAugmentedRayList },
		{ 8, &DrawingSet::GetAugmentedPathList },
		{ 1, &DrawingSet::GetPointList },
		{ 2, &DrawingSet::GetLineList },
		{ 3, &DrawingSet::GetAreaList },
		{ 5, &DrawingSet::GetTextList },
	};

	// A drawing set has a list for each of the priorities 0 to 99.
	const int priorityCount = 100;

	// The curves a line instruction draws: the ones its spatial references name, or all of its
	// geometry.
	void CurvesOf(SENC_LineInstruction* instruction, std::list<SCurve*>& curves)
	{
		auto geom = instruction->fr->GetGeometry();

		if (instruction->spatialReference.size() > 0)
		{
			if (geom->GetType() == SGeometryType::Surface)
			{
				auto surface = (SSurface*)geom;
				for (SENC_SpatialReference* reference : instruction->spatialReference)
				{
					if (auto curve = surface->GetCurve(reference->reference))
					{
						curves.push_back(curve);
					}
				}
			}
			else if (geom->GetType() == SGeometryType::CompositeCurve)
			{
				auto compositeCurve = (SCompositeCurve*)geom;
				for (SENC_SpatialReference* reference : instruction->spatialReference)
				{
					if (auto curve = compositeCurve->GetCurveByRCID(reference->reference))
					{
						curves.push_back(curve);
					}
				}
			}
			else if (geom->GetType() == SGeometryType::Curve)
			{
				auto curve = (SCurve*)geom;
				for (SENC_SpatialReference* reference : instruction->spatialReference)
				{
					if (curve->GetRCID() == reference->reference)
					{
						curves.push_back(curve);
					}
				}
			}
		}
		else if (geom->GetType() == SGeometryType::Surface)
		{
			((SSurface*)geom)->GetCurveList(curves);
		}
		else if (geom->GetType() == SGeometryType::CompositeCurve)
		{
			((SCompositeCurve*)geom)->GetCurveList(curves);
		}
		else if (geom->GetType() == SGeometryType::Curve)
		{
			curves.push_back((SCurve*)geom);
		}
	}
}

namespace s100::render
{
	void CollectInstructions(SENC_DisplayList& displayList, int scale, Scaler& scaler, const FeatureFilter& shown,
		DrawingSet& drawingSet, std::set<int>& priorities)
	{
		std::vector<SENC_Instruction*> found;
		for (int priority = 0; priority < priorityCount; priority++)
		{
			bool any = false;
			for (const Kind& kind : kinds)
			{
				displayList.GetDrawingInstruction(priority, kind.type, scale, &scaler, found);
				if (found.empty())
				{
					continue;
				}
				auto list = (drawingSet.*kind.list)(priority);
				for (SENC_Instruction* instruction : found)
				{
					if (shown && !shown(instruction->code))
					{
						continue;
					}
					list->push_back(instruction);
					any = true;
				}
			}
			if (any)
			{
				priorities.insert(priority);
			}
		}
	}

	void SuppressSharedCurves(const std::set<int>& priorities, DrawingSet& drawingSet)
	{
		std::unordered_set<int> drawn;   // the record identifiers of the curves that have an instruction
		drawn.reserve(4096);

		// From the highest priority down.
		for (auto priority = priorities.rbegin(); priority != priorities.rend(); priority++)
		{
			std::list<SCurve*> curves;
			for (SENC_Instruction* instruction : *drawingSet.GetLineList(*priority))
			{
				curves.clear();
				CurvesOf((SENC_LineInstruction*)instruction, curves);

				for (SCurve* curve : curves)
				{
					if (false == curve->GetMasking())
					{
						if (!drawn.insert(curve->GetRCID()).second)
						{
							curve->SetSuppress(true);
						}
					}
				}
			}
		}
	}

	void PaintPriority(int priority, DrawingSet& drawingSet, SENC_InstructionVisitor& painter, bool texts,
		DrawingStages* stages)
	{
		DrawingStages none;
		DrawingStages& stage = stages ? *stages : none;

		stage.BeginAreasAndLines();
		for (SENC_Instruction* instruction : *drawingSet.GetAreaList(priority))
		{
			stage.BeforeArea(*instruction);
			instruction->Accept(painter);
		}
		for (SENC_Instruction* instruction : *drawingSet.GetLineList(priority))
		{
			stage.BeforeLine(*instruction);
			instruction->Accept(painter);
		}
		stage.EndAreasAndLines();

		for (SENC_Instruction* instruction : *drawingSet.GetAugmentedRayList(priority))
		{
			instruction->Accept(painter);
		}
		for (SENC_Instruction* instruction : *drawingSet.GetAugmentedPathList(priority))
		{
			instruction->Accept(painter);
		}
		for (SENC_Instruction* instruction : *drawingSet.GetPointList(priority))
		{
			instruction->Accept(painter);
		}

		if (texts)
		{
			auto& list = *drawingSet.GetTextList(priority);
			stage.BeginTexts(!list.empty());
			for (SENC_Instruction* instruction : list)
			{
				stage.BeforeText(*instruction);
				instruction->Accept(painter);
			}
			stage.EndTexts();
		}
	}
}
