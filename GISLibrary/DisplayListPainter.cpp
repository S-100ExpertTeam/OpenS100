#include "stdafx.h"
#include "DisplayListPainter.h"

#include "R_FeatureRecord.h"
#include "SENC_ArcByRadius.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AugmentedRay.h"
#include "SENC_Dash.h"
#include "SENC_LineStyle.h"
#include "SENC_Path.h"
#include "SENC_Sector.h"
#include "SGeometry.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../LatLonUtility/LatLonUtility.h"

#include <cmath>
#include <utility>

// The arithmetic below is the arithmetic of the Direct2D painter it came from, type for type:
// the pictures are compared byte for byte.
#pragma warning(disable:4244)
#pragma warning(disable:4838)

namespace
{
	// Millimetres of an augmented ray or arc to pixels.
	const float factor = 3.5;
}

namespace s100::render
{
	DisplayListPainter::DisplayListPainter(IRenderer& renderer, Scaler& scaler, ColorLookup color)
		: renderer(renderer), scaler(scaler), color(std::move(color))
	{
	}

	// A ray from a point feature: a line of a given length in a given direction from north
	// (a leading line, a sector limit of a light).
	void DisplayListPainter::Visit(SENC_AugmentedRay& instruction)
	{
		auto& fr = instruction.fr;
		auto& lineStyles = instruction.lineStyles;
		auto& direction = instruction.direction;
		auto& length = instruction.length;

		if (nullptr == fr || nullptr == fr->GetGeometry())
		{
			return;
		}

		auto geom = fr->GetGeometry();
		if (geom->GetType() != SGeometryType::Point)
		{
			return;
		}

		auto p = (SPoint*)geom;

		scaler.WorldToDevice(
			p->x,
			p->y,
			&SGeometry::viewPoints[0].x,
			&SGeometry::viewPoints[0].y);

		for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
		{
			auto lineStyle = *i;
			if (!lineStyle->IsLineStyle())
			{
				continue;   // a reference or a composite style: not drawn (as before)
			}

			SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;
			int PEN_WIDTH = (int)(ls->pen_width / 0.319);

			PointF pBeginningPt = { (float)SGeometry::viewPoints[0].x, (float)SGeometry::viewPoints[0].y };
			PointF pEndPt;

			// Point rotate
			if (scaler.GetRotationDegree())
			{
				double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

				float tempX = (scaler.soy - pBeginningPt.y) * sin(radian) + (scaler.sox - pBeginningPt.x) * cos(radian);
				float tempY = (scaler.soy - pBeginningPt.y) * cos(radian) - (scaler.sox - pBeginningPt.x) * sin(radian);

				pBeginningPt.x = tempX + scaler.sox;
				pBeginningPt.y = tempY + scaler.soy;
			}

			long ix = 0;
			long iy = 0;
			double mx = p->x;
			double my = p->y;

			if (false == ENCCommon::FULL_SECTORS)
			{
				// The direction is from north, which is not up about a pole.
				const double onScreen = direction + NorthAngle(mx, my) - 90;
				ix = pBeginningPt.x + (int)((cos(onScreen * M_PI / 180) * length) * factor);
				iy = pBeginningPt.y + (int)((sin(onScreen * M_PI / 180) * length) * factor);

				// Point rotate
				if (scaler.GetRotationDegree())
				{
					double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

					float tempX = (pBeginningPt.y - iy) * sin(radian) + (pBeginningPt.x - ix) * cos(radian);
					float tempY = (pBeginningPt.y - iy) * cos(radian) - (pBeginningPt.x - ix) * sin(radian);

					ix = tempX + pBeginningPt.x;
					iy = tempY + pBeginningPt.y;
				}
			}
			else
			{
				inverseProjection(mx, my);
				auto dest = LatLonUtility::GetDestinationPointRhumbLineMercatorByGeographicLib(my, mx, direction, length);
				projection(dest.lon, dest.lat);
				scaler.WorldToDevice(dest.lon, dest.lat, &ix, &iy);

				// Point rotate
				if (scaler.GetRotationDegree())
				{
					double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

					float tempX = (scaler.soy - iy) * sin(radian) + (scaler.sox - ix) * cos(radian);
					float tempY = (scaler.soy - iy) * cos(radian) - (scaler.sox - ix) * sin(radian);

					ix = tempX + scaler.sox;
					iy = tempY + scaler.soy;
				}
			}

			pEndPt.x = (float)ix;
			pEndPt.y = (float)iy;

			PathFigure line;
			line.start = pBeginningPt;
			line.lines.push_back(pEndPt);
			Path path;
			path.figures.push_back(line);

			Stroke stroke;
			stroke.width = PEN_WIDTH;
			if (ls->dashes.size() > 0)
			{
				float dashes[] = { (ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH), (ls->intervalLength - ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH) };
				stroke.dashes.assign(std::begin(dashes), std::end(dashes));
			}

			renderer.StrokePath(path, color(ls->GetPenColorToken()), stroke);
		}
	}

	// Arcs about a point feature (the sectors of a light), turned to where north is at the point.
	void DisplayListPainter::Visit(SENC_AugmentedPath& instruction)
	{
		auto& fr = instruction.fr;
		auto& path = instruction.path;
		auto& lineStyles = instruction.lineStyles;

		if (!fr || !fr->GetGeometry() || !path)
		{
			return;
		}

		auto geom = fr->GetGeometry();
		if (geom->GetType() != SGeometryType::Point)
		{
			return;
		}

		SPoint* p = (SPoint*)geom;
		scaler.WorldToDevice(p->x, p->y, &SGeometry::viewPoints[0].x, &SGeometry::viewPoints[0].y);

		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase* lineStyle = *itor;
			if (lineStyle->type != 1)
			{
				continue;   // a reference or a composite style: not drawn (as before)
			}

			SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;

			// One path an arc, about the origin with north up.
			std::vector<Path> arcs;
			for (auto _itor = path->arcByRadiuses.begin(); _itor != path->arcByRadiuses.end(); _itor++)
			{
				SENC_ArcByRadius* _arcByRadius = *_itor;
				if (!_arcByRadius->sector)
				{
					continue;
				}

				float displayRadius = _arcByRadius->radius * 3.5;

				// A sector starts from north (up the screen); an arc angle is counted from
				// straight down.
				float startAngle = _arcByRadius->sector->startAngle + 180;
				float endAngle = startAngle + _arcByRadius->sector->angularDistance;

				Path arc;
				arc.arcs.push_back({ displayRadius, startAngle, endAngle });
				arcs.push_back(std::move(arc));
			}

			// The arcs are built from north up: turned to where north is at the point.
			renderer.SetTransform({
				TransformStep::Rotate(static_cast<float>(NorthAngle(p->x, p->y))),
				TransformStep::Translate(SGeometry::viewPoints[0].x, SGeometry::viewPoints[0].y) });

			Stroke stroke;
			stroke.width = ls->pen_width / 0.32;
			if (ls->dashes.size() > 0)
			{
				float dashes[] = {
					(float)(ls->dashes.front()->length + 0.01) / (float)0.32,
					(float)(ls->intervalLength - ls->dashes.front()->length + 0.01) / (float)0.32
				};
				stroke.dashes.assign(std::begin(dashes), std::end(dashes));
			}

			const ColorF penColor = color(ls->GetPenColorToken());
			for (const Path& arc : arcs)
			{
				renderer.StrokePath(arc, penColor, stroke);
			}
		}
	}
}
