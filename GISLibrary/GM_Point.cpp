#include "stdafx.h"
#include "GM_Point.h"

namespace GM
{
	Point::Point()
	{

	}

	Point::Point(const Point& item) : Primitive(item)
	{
		this->position = item.position;
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

	double Point::getX()
	{
		return position.GetX();
	}
	double Point::getY()
	{
		return position.GetY();
	}

	GeometryType Point::GetType()
	{
		return GeometryType::Point;
	}

	std::string Point::GeometricPrimitiveAsString()
	{
		return "Point";
	}

	SGeometry* Point::createGeometry()
	{
		auto x = position.GetX();
		auto y = position.GetY();
		projection(x, y);

		SPoint* result = new SPoint(x, y);
		return result;
	}
}