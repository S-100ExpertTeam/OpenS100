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
		CompositeCurve(CompositeCurve& item);
		virtual ~CompositeCurve();

	public:
		std::vector<OrientableCurve*> component;

	public:
		GeometryType GetType() override;

		int getPointCount() override;
		double getX(int index) override;
		double getY(int index) override;

	public:
		void Add(std::string curveMemberID);
		void Add(GM::OrientableCurve* item);
		void Add(GM::Curve* item);
		void Add(GM::CompositeCurve* item);
	};
}