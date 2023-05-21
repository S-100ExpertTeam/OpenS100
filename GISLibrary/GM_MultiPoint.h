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
		virtual ~MultiPoint();

	public:
		std::vector<DirectPosition> position;

	public:
		void Set(double x, double y, double z);

		GeometryType GetType() override;
	};
}