#pragma once

#include "GM_Primitive.h"
#include "DirectPosition.h"
#include "PugixmlElement.h"

#include <string>

namespace GM
{
	class Point : 
		public Primitive
	{
	public:
		Point();
		Point(double x, double y);
		Point(double x, double y, double z);
		Point(const Point& item);
		virtual ~Point();

	public:
		DirectPosition position;

	public:
		void Read(pugi::xml_node& node);
		void Write(pugi::xml_node& node);

		double getX();
		double getY();
		double getZ();

		void setX(double x);
		void setY(double y);
		void setZ(double z);

	public:
		GeometryType GetType() override;
		std::string GeometricPrimitiveAsString() override;
		SGeometry* createGeometry();
	};
}