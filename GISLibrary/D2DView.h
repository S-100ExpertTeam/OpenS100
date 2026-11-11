#pragma once
// Direct2D view helpers derived from the platform-neutral Scaler (render side).
//
// Scaler keeps map<->screen math in plain numbers; anything that needs a Direct2D matrix,
// rectangle or a Win32 RECT asks here. Phase 1 step 3-4.
#include <windows.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include "../GeoMetryLibrary/Scaler.h"

namespace GISLibrary
{
	namespace D2DView
	{
		// Device pixels per world unit, as Scaler::WorldToDevice has it: the map range fits the
		// screen in both directions, so the larger of the two world-per-pixel ratios wins.
		inline double PixelsPerWorldUnit(const Scaler& scaler)
		{
			const double xRatio = (scaler.mxMax - scaler.mxMin) / (scaler.sxMax - scaler.sxMin);
			const double yRatio = (scaler.myMax - scaler.myMin) / (scaler.syMax - scaler.syMin);
			return 1.0 / (xRatio > yRatio ? xRatio : yRatio);
		}

		// World -> device transform of the current view, for geometry in world coordinates with y
		// negated (D2DGeometryCache); the same mapping as Scaler::WorldToDevice.
		inline D2D1::Matrix3x2F Matrix(const Scaler& scaler)
		{
			const double mox = (scaler.mxMax + scaler.mxMin) / 2;
			const double moy = (scaler.myMax + scaler.myMin) / 2;
			const double sox = (scaler.sxMax + scaler.sxMin) / 2;
			const double soy = (scaler.syMax + scaler.syMin) / 2;
			const double ratio = PixelsPerWorldUnit(scaler);
			const auto translation1 = D2D1::Matrix3x2F::Translation(static_cast<float>(-mox), static_cast<float>(moy));
			const auto scale = D2D1::Matrix3x2F::Scale(static_cast<float>(ratio), static_cast<float>(ratio));
			const auto translation2 = D2D1::Matrix3x2F::Translation(static_cast<float>(sox), static_cast<float>(soy));
			return translation1 * scale * translation2;
		}

		// The screen in world coordinates with y negated, as Matrix maps them.
		inline D2D1_RECT_F WorldRect(const Scaler& scaler)
		{
			const double mox = (scaler.mxMax + scaler.mxMin) / 2;
			const double moy = (scaler.myMax + scaler.myMin) / 2;
			const double ratio = PixelsPerWorldUnit(scaler);
			const double halfWidth = (scaler.sxMax - scaler.sxMin) / ratio / 2;
			const double halfHeight = (scaler.syMax - scaler.syMin) / ratio / 2;
			return D2D1::RectF(static_cast<float>(mox - halfWidth), static_cast<float>(-moy - halfHeight),
				static_cast<float>(mox + halfWidth), static_cast<float>(-moy + halfHeight));
		}

		inline D2D1::Matrix3x2F InverseMatrix(const Scaler& scaler)
		{
			auto matrix = Matrix(scaler);
			matrix.Invert();
			return matrix;
		}

		inline D2D1_RECT_F Rect(const Scaler& scaler)
		{
			return D2D1::RectF(0.0f, 0.0f, static_cast<float>(scaler.GetScreenWidth()), static_cast<float>(scaler.GetScreenHeight()));
		}

		inline RECT ScreenRECT(const Scaler& scaler)
		{
			const auto r = scaler.GetScreenRect();
			return RECT{ r.left, r.top, r.right, r.bottom };
		}
	}
}
