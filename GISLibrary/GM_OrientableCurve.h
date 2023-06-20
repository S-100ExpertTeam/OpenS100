#pragma once

#include "GM_Primitive.h"

#include "DirectPosition.h"

#include <string>

namespace GM
{
	class OrientableCurve : public Primitive
	{
	public:
		OrientableCurve();
		OrientableCurve(std::string baseCurveID);
		virtual ~OrientableCurve();

	public:
		// true : +
		// false : -
		bool orientation = false; 
		std::string baseCurveID;

	public:
		GeometryType GetType() override;

		int GetBaseCurveIDAsInt();
	};
}