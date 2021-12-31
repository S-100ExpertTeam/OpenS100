#pragma once
#include "figure.h"

namespace libS100Engine
{
	class Rectangle : Figure
	{
	public:
		Rectangle();
		Rectangle(POINTF posLeftTop, POINTF posRightBottom);
		virtual ~Rectangle();

	public:
		POINTF m_posLeftTop;
		POINTF m_posRightBottom;
	};
}