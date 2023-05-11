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
		SurfacePatch* patch = new Polygon();

	public:
		Polygon* GetPolygon();
	};
}