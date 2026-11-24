#include "stdafx.h"
#include "DepthMaskStages.h"

#include "GF_FeatureType.h"
#include "SENC_Instruction.h"

#include "../GeoMetryLibrary/Scaler.h"

namespace s100::render
{
	bool IsDepthFeatureUnderCoverage(std::string_view featureCode) noexcept
	{
		return featureCode == "DepthArea" || featureCode == "DredgedArea" || featureCode == "DepthContour";
	}

	bool IsDepthFeatureUnderCoverage(const SENC_Instruction& instruction)
	{
		const std::wstring& code = instruction.code;
		if (!code.empty())
		{
			return code == L"DepthArea" || code == L"DredgedArea" || code == L"DepthContour";
		}
		return instruction.fr && IsDepthFeatureUnderCoverage(instruction.fr->GetCode());
	}

	DepthMaskStages::DepthMaskStages(IRenderer& renderer, Scaler& scaler, const WorldRings& rings)
		: renderer(renderer), scaler(scaler), rings(rings)
	{
	}

	DepthMaskStages::~DepthMaskStages()
	{
		Close();
	}

	void DepthMaskStages::BeforeArea(SENC_Instruction& instruction)
	{
		Update(instruction);
	}

	void DepthMaskStages::BeforeLine(SENC_Instruction& instruction)
	{
		Update(instruction);
	}

	void DepthMaskStages::EndAreasAndLines()
	{
		Close();
	}

	void DepthMaskStages::BeforeText(SENC_Instruction& instruction)
	{
		Update(instruction);
	}

	void DepthMaskStages::EndTexts()
	{
		Close();
	}

	void DepthMaskStages::Close()
	{
		if (open)
		{
			renderer.PopClip();
			open = false;
		}
	}

	void DepthMaskStages::Update(const SENC_Instruction& instruction)
	{
		if (rings.empty())
		{
			return;
		}
		const bool suppressed = IsDepthFeatureUnderCoverage(instruction);
		if (suppressed == open)
		{
			return;
		}
		if (!suppressed)
		{
			Close();
			return;
		}

		if (!made)
		{
			// Under the even-odd rule the data areas are holes in a rectangle over the view, with
			// a margin of the view's own size each side.
			const float width = static_cast<float>(scaler.GetScreenWidth());
			const float height = static_cast<float>(scaler.GetScreenHeight());
			PathFigure outer;
			outer.start = { -width, -height };
			outer.lines = { { 2 * width, -height }, { 2 * width, 2 * height }, { -width, 2 * height } };
			outer.closed = true;
			mask.figures.push_back(std::move(outer));
			for (const auto& ring : rings)
			{
				PathFigure figure;
				bool first = true;
				for (const WorldPoint& point : ring)
				{
					float x = 0.0f;
					float y = 0.0f;
					scaler.WorldToDevice_F(point.x, point.y, &x, &y);
					if (first)
					{
						figure.start = { x, y };
						first = false;
					}
					else
					{
						figure.lines.push_back({ x, y });
					}
				}
				if (!first)
				{
					figure.closed = true;
					mask.figures.push_back(std::move(figure));
				}
			}
			made = true;
		}

		// The mask is in device pixels; the painter sets the transform of what it draws itself.
		renderer.SetTransform({});
		renderer.PushClip(mask);
		open = true;
	}
}
