#pragma once
#include "Declaration.h"

#include <string>
#include <list>

namespace S100_SVG_D2D1_DLL
{
	class CSSStyle;
	class UnitOfCSS
	{
	public:
		UnitOfCSS();
		virtual ~UnitOfCSS();

	public:
		std::wstring selector;
		std::list<Declaration> declaration;
		std::list<CSSStyle*> styles;
	};
}