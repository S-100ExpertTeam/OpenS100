#pragma once

#include "GM_OrientableCurve.h"
#include "GM_Curve.h"

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
		void Add(GM::OrientableCurve item);
		void Add(GM::Curve item);
		void Add(GM::CompositeCurve item);
	};
}