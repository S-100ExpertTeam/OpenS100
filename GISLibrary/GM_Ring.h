#pragma once

#include "GM_CompositeCurve.h"

namespace GM
{
	class Ring : public CompositeCurve
	{
	public:
		Ring();
		Ring(const Ring& item);
		Ring(GM::CompositeCurve& curve);
		Ring(GM::OrientableCurve& curve);
		Ring(GM::Curve& curve);
		virtual ~Ring();
	};
}