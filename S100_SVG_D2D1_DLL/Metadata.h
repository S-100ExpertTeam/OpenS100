#pragma once
#include "IHO_S100SVG.h"

namespace S100_SVG_D2D1_DLL
{
	class Metadata
	{
	public:
		Metadata();
		virtual ~Metadata();

	public:
		IHO_S100SVG s100svg;
	};
}