#pragma once

#include "GM_OrientableCurve.h"

#include <vector>

namespace GM
{
	class CompositeCurve : public OrientableCurve
	{
	public:
		CompositeCurve();
		virtual ~CompositeCurve();

	public:
		std::vector<OrientableCurve> component;

	public:
		GeometryType GetType() override;

		void Add(std::string curveMemberID);
	};
}