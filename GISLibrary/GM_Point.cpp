#include "stdafx.h"
#include "GM_Point.h"

namespace GM
{
	Point::Point()
	{

	}

	Point::~Point()
	{

	}

	void Point::Read(pugi::xml_node& node)
	{

	}

	void Point::Write(pugi::xml_node& node)
	{

	}

	GeometryType Point::GetType()
	{
		return GeometryType::Point;
	}
}