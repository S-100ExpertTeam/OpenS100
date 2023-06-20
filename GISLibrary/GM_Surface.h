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
		void AddInteriorRingID(std::string value);

		GeometryType GetType() override;
	};
}