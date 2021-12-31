#pragma once
#include "IHO_Description.h"
#include <string>

namespace S100_SVG_D2D1_DLL
{
	class IHO_S100SVG
	{
	public:
		IHO_S100SVG();
		virtual ~IHO_S100SVG();

	public:
		std::wstring xmlns;
		IHO_Description desc;
	};
}