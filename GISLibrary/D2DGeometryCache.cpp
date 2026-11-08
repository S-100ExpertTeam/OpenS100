#include "stdafx.h"
#include "D2DGeometryCache.h"
#include "SCurve.h"
#include "SSurface.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <memory>
#include <vector>

namespace
{
	// What the renderer parks in Geometry::renderAttachment.
	struct D2DAttachment : public Geometry::RenderAttachment
	{
		ID2D1Geometry* geometry = nullptr;

		~D2DAttachment() override
		{
			if (geometry)
			{
				geometry->Release();
				geometry = nullptr;
			}
		}
	};

	D2DAttachment* Slot(Geometry& model)
	{
		auto existing = dynamic_cast<D2DAttachment*>(model.renderAttachment.get());
		if (!existing)
		{
			model.renderAttachment = std::make_unique<D2DAttachment>();
			existing = static_cast<D2DAttachment*>(model.renderAttachment.get());
		}
		return existing;
	}

	ID2D1PathGeometry* BuildPath(ID2D1Factory1* factory, const std::vector<D2D1_POINT_2F>& points, bool filled)
	{
		if (points.size() < 2)
		{
			return nullptr;
		}
		ID2D1PathGeometry* path = nullptr;
		if (FAILED(factory->CreatePathGeometry(&path)) || !path)
		{
			return nullptr;
		}
		ID2D1GeometrySink* sink = nullptr;
		if (FAILED(path->Open(&sink)) || !sink)
		{
			path->Release();
			return nullptr;
		}
		sink->BeginFigure(points[0], filled ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);
		sink->AddLines(points.data() + 1, static_cast<UINT32>(points.size() - 1));
		sink->EndFigure(filled ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
		sink->Close();
		sink->Release();
		return path;
	}

	// Curve points: world (x, -y) or device space depending on scaler.
	std::vector<D2D1_POINT_2F> CurvePoints(SCurve* curve, Scaler* scaler)
	{
		std::vector<D2D1_POINT_2F> points(static_cast<size_t>(curve->m_numPoints));
		for (int i = 0; i < curve->m_numPoints; i++)
		{
			if (scaler)
			{
				scaler->WorldToDevice_F(curve->m_pPoints[i].x, curve->m_pPoints[i].y, &points[i].x, &points[i].y);
			}
			else
			{
				points[i].x = static_cast<float>(curve->m_pPoints[i].x);
				points[i].y = static_cast<float>(-curve->m_pPoints[i].y);
			}
		}
		return points;
	}

	std::vector<D2D1_POINT_2F> SurfacePoints(SSurface* surface, Scaler* scaler)
	{
		std::vector<D2D1_POINT_2F> points(static_cast<size_t>(surface->m_numPoints));
		for (int i = 0; i < surface->m_numPoints; i++)
		{
			if (scaler)
			{
				scaler->WorldToDevice_F(surface->m_pPoints[i].x, surface->m_pPoints[i].y, &points[i].x, &points[i].y);
			}
			else
			{
				points[i].x = static_cast<float>(surface->m_pPoints[i].x);
				points[i].y = static_cast<float>(-surface->m_pPoints[i].y);
			}
		}
		return points;
	}

	// Single ring -> path geometry; several rings -> geometry group (alternate fill for holes).
	ID2D1Geometry* BuildSurface(SSurface* surface, ID2D1Factory1* factory, Scaler* scaler)
	{
		if (!factory || surface->m_numPoints < 3)
		{
			return nullptr;
		}
		const auto points = SurfacePoints(surface, scaler);

		if (surface->m_numParts <= 1)
		{
			return BuildPath(factory, points, true);
		}

		std::vector<ID2D1Geometry*> rings;
		rings.reserve(static_cast<size_t>(surface->m_numParts));
		for (int part = 0; part < surface->m_numParts; part++)
		{
			const int count = surface->GetNumPointPerPart(part);
			if (count < 2)
			{
				continue;
			}
			const int start = surface->m_pParts[part];
			std::vector<D2D1_POINT_2F> ring(points.begin() + start, points.begin() + start + count);
			if (auto path = BuildPath(factory, ring, true))
			{
				rings.push_back(path);
			}
		}

		ID2D1GeometryGroup* group = nullptr;
		if (!rings.empty())
		{
			factory->CreateGeometryGroup(D2D1_FILL_MODE_ALTERNATE, rings.data(), static_cast<UINT32>(rings.size()), &group);
		}
		for (auto ring : rings)
		{
			ring->Release();
		}
		return group;
	}
}

namespace GISLibrary
{
	namespace D2DGeometry
	{
		ID2D1PathGeometry* Get(SCurve* curve, ID2D1Factory1* factory)
		{
			if (!curve || !factory)
			{
				return nullptr;
			}
			auto slot = Slot(*curve);
			if (!slot->geometry)
			{
				slot->geometry = BuildPath(factory, CurvePoints(curve, nullptr), false);
			}
			return static_cast<ID2D1PathGeometry*>(slot->geometry);
		}

		ID2D1Geometry* Get(SSurface* surface, ID2D1Factory1* factory)
		{
			if (!surface || !factory)
			{
				return nullptr;
			}
			auto slot = Slot(*surface);
			if (!slot->geometry)
			{
				slot->geometry = BuildSurface(surface, factory, nullptr);
			}
			return slot->geometry;
		}

		ID2D1PathGeometry* CreateScaled(SCurve* curve, ID2D1Factory1* factory, Scaler* scaler)
		{
			if (!curve || !factory || !scaler)
			{
				return nullptr;
			}
			return BuildPath(factory, CurvePoints(curve, scaler), false);
		}

		ID2D1Geometry* CreateScaled(SSurface* surface, ID2D1Factory1* factory, Scaler* scaler)
		{
			if (!surface || !factory || !scaler)
			{
				return nullptr;
			}
			return BuildSurface(surface, factory, scaler);
		}
	}
}
