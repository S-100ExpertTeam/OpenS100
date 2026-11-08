#pragma once
// Direct2D rendering of S-102 bathymetry grids (render side).
//
// S102H5 is a dataset: it reads the HDF5 grid and knows nothing about render targets.
// This renderer rasterises the grid once into the dataset's WIC bitmap cache and blits it
// for the current view. Step 2-2 of Documents/TechDocs/2026-11-08-phase1-core-decoupling.md.
#include <windows.h>

class S102H5;
class Scaler;

namespace GISLibrary
{
	class D2D1Resources;

	namespace S102Renderer
	{
		void Draw(S102H5& grid, D2D1Resources* d2, HDC& hDC, Scaler* scaler);
	}
}
