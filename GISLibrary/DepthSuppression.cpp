#include "stdafx.h"
#include "DepthSuppression.h"

#include "CoverageDataset.h"
#include "D2DView.h"
#include "GF_FeatureType.h"
#include "SENC_Instruction.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <d2d1_1.h>

namespace GISLibrary::DepthSuppression
{
	namespace
	{
		struct CachedMask
		{
			std::uint64_t version = 0;
			ID2D1Factory1* factory = nullptr;
			ID2D1PathGeometry* geometry = nullptr;   // null: nothing suppressed

			~CachedMask()
			{
				if (geometry) geometry->Release();
			}
		};

		CachedMask& cache()
		{
			static CachedMask mask;
			return mask;
		}
	}

	bool IsSuppressedFeature(std::string_view featureCode) noexcept
	{
		return featureCode == "DepthArea" || featureCode == "DredgedArea" || featureCode == "DepthContour";
	}

	ID2D1Geometry* DataAreas(ID2D1Factory1* factory)
	{
		CachedMask& mask = cache();
		const std::uint64_t version = CoverageDataset::SuppressionVersion();
		if (mask.version == version && mask.factory == factory)
		{
			return mask.geometry;
		}
		if (mask.geometry)
		{
			mask.geometry->Release();
			mask.geometry = nullptr;
		}
		mask.version = version;
		mask.factory = factory;

		const auto rings = CoverageDataset::SuppressionRings();
		if (rings.empty() || !factory)
		{
			return nullptr;
		}
		ID2D1PathGeometry* geometry = nullptr;
		ID2D1GeometrySink* sink = nullptr;
		if (FAILED(factory->CreatePathGeometry(&geometry)) || FAILED(geometry->Open(&sink)))
		{
			if (geometry) geometry->Release();
			return nullptr;
		}
		sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
		for (const auto& ring : rings)
		{
			bool first = true;
			for (const auto& position : ring)
			{
				double x = position.lon;
				double y = position.lat;
				projection(x, y);
				// World coordinates are drawn with y up, as D2DGeometryCache builds them.
				const D2D1_POINT_2F point = D2D1::Point2F(static_cast<float>(x), static_cast<float>(-y));
				if (first)
				{
					sink->BeginFigure(point, D2D1_FIGURE_BEGIN_FILLED);
					first = false;
				}
				else
				{
					sink->AddLine(point);
				}
			}
			if (!first)
			{
				sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			}
		}
		const bool closed = SUCCEEDED(sink->Close());
		sink->Release();
		if (!closed)
		{
			geometry->Release();
			return nullptr;
		}
		mask.geometry = geometry;
		return mask.geometry;
	}

	Runs::Runs(ID2D1RenderTarget* target, ID2D1Factory1* factory, const Scaler& scaler, bool worldTransform)
		: target_(target), scaler_(&scaler), worldTransform_(worldTransform)
	{
		ID2D1Geometry* areas = target ? DataAreas(factory) : nullptr;
		if (!areas)
		{
			return;
		}
		// The screen less the S-102 data areas: under the alternate (even-odd) rule the data areas
		// are holes in a rectangle over the screen, with a margin of the screen's own size each side.
		const D2D1_RECT_F screen = D2DView::WorldRect(scaler);
		const float width = screen.right - screen.left;
		const float height = screen.bottom - screen.top;
		ID2D1RectangleGeometry* outer = nullptr;
		if (FAILED(factory->CreateRectangleGeometry(D2D1::RectF(screen.left - width, screen.top - height, screen.right + width, screen.bottom + height), &outer)))
		{
			return;
		}
		ID2D1Geometry* parts[] = { outer, areas };
		ID2D1GeometryGroup* group = nullptr;
		if (SUCCEEDED(factory->CreateGeometryGroup(D2D1_FILL_MODE_ALTERNATE, parts, 2, &group)))
		{
			mask_ = group;
		}
		outer->Release();
	}

	Runs::~Runs()
	{
		Close();
		if (layer_) layer_->Release();
		if (mask_) mask_->Release();
	}

	void Runs::Close()
	{
		if (open_)
		{
			target_->PopLayer();
			open_ = false;
		}
	}

	void Runs::Update(const SENC_Instruction* instruction)
	{
		if (!mask_)
		{
			return;
		}
		bool suppressed = false;
		if (instruction)
		{
			const std::wstring& code = instruction->code;
			suppressed = !code.empty() ? (code == L"DepthArea" || code == L"DredgedArea" || code == L"DepthContour")
				: instruction->fr && IsSuppressedFeature(instruction->fr->GetCode());
		}
		if (suppressed == open_)
		{
			return;
		}
		if (!suppressed)
		{
			Close();
			return;
		}
		if (!layer_ && FAILED(target_->CreateLayer(&layer_)))
		{
			mask_->Release();
			mask_ = nullptr;
			return;
		}
		// The mask is in world coordinates: the target's world transform carries it to the device
		// when the target draws in world coordinates; otherwise the mask transform does.
		const D2D1_MATRIX_3X2_F maskTransform = worldTransform_ ? D2D1::Matrix3x2F::Identity() : D2DView::Matrix(*scaler_);
		target_->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), mask_, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, maskTransform), layer_);
		open_ = true;
	}
}
