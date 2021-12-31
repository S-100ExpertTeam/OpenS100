#include "stdafx.h"
#include "Rectangle.h"

namespace libS100Engine
{
	Rectangle::Rectangle()
	{

	}

	Rectangle::Rectangle(POINTF posLeftTop, POINTF posRightBottom)
	{
		m_posLeftTop = posLeftTop;
		m_posRightBottom = posRightBottom;
	}

	Rectangle::~Rectangle()
	{

	}
}