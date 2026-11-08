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
		// World -> device transform of the current view.
		inline D2D1::Matrix3x2F Matrix(const Scaler& scaler)
		{
			const auto translation1 = D2D1::Matrix3x2F::Translation(static_cast<float>(-scaler.mox), static_cast<float>(scaler.moy));
			const double ratio = (scaler.sxMax - scaler.sxMin) / (scaler.mxMax - scaler.mxMin);
			const auto scale = D2D1::Matrix3x2F::Scale(static_cast<float>(ratio), static_cast<float>(ratio));
			const auto translation2 = D2D1::Matrix3x2F::Translation(static_cast<float>(scaler.sox), static_cast<float>(scaler.soy));
			return translation1 * scale * translation2;
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
