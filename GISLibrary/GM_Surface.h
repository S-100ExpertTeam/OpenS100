#pragma once

#include "GM_OrientableSurface.h"
#include "GM_SurfacePatch.h"
#include "GM_Polygon.h"

namespace GM
{
	class Surface : public OrientableSurface
	{
	public:
		Surface();
		~Surface();

	public:
		Polygon patch;

	public:
		Polygon GetPolygon();
		
		void SetExteriorRingID(std::string value);
		void SetExteriorRing(GM::OrientableCurve item);
		void SetExteriorRing(GM::Curve item);
		void SetExteriorRing(GM::CompositeCurve item);
		void AddInteriorRingID(std::string value);
		void AddInteriorRing(GM::OrientableCurve item);
		void AddInteriorRing(GM::Curve item);
		void AddInteriorRing(GM::CompositeCurve item);

		GeometryType GetType() override;
		std::string GeometricPrimitiveAsString() override;
	};
}