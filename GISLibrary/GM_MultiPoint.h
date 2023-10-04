#pragma once

#include "GM_Object.h"
#include "DirectPosition.h"

#include <vector>

namespace GM
{
	class MultiPoint : public Object
	{
	public:
		MultiPoint();
		MultiPoint(const MultiPoint& item);
		virtual ~MultiPoint();

	public:
		std::vector<DirectPosition> position;

	public:
		void Add(double x, double y, double z);

	public:
		GeometryType GetType() override;
		std::string GeometricPrimitiveAsString() override;
		SGeometry* createGeometry();
	};
}