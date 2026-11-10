#include "stdafx.h"
#include "ViewSelection.h"

#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/MBR.h"
#include "../GeoMetryLibrary/Scaler.h"

namespace GISLibrary
{
	std::optional<s100::sdb::DatasetSelector::View> selectionView(Scaler& scaler)
	{
		const int width = scaler.GetScreenWidth();
		const int height = scaler.GetScreenHeight();
		const int scale = scaler.GetCurrentScale();
		if (width <= 0 || height <= 0 || scale <= 0)
		{
			return std::nullopt;
		}

		s100::sdb::DatasetSelector::View view{
			.scale = static_cast<double>(scale),
			.width = static_cast<double>(width),
			.height = static_cast<double>(height),
		};
		view.project = [&scaler](const s100::sdb::Position& position) {
			double x = position.lon;
			double y = position.lat;
			projection(x, y);
			float deviceX = 0;
			float deviceY = 0;
			scaler.WorldToDevice_F(x, y, &deviceX, &deviceY);
			return s100::sdb::DevicePoint{ deviceX, deviceY };
		};
		if (scaler.GetRotationDegree() == 0)
		{
			MBR map;
			scaler.GetMap(&map);
			inverseProjection(map);
			view.ground = s100::sdb::BoundingBox{ map.xmin, map.ymin, map.xmax, map.ymax };
		}
		return view;
	}
}
