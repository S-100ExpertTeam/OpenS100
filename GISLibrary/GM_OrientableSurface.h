#pragma once

#include "GM_Primitive.h"

namespace GM
{
	class OrientableSurface : public Primitive
	{
	public:
		OrientableSurface();
		virtual ~OrientableSurface();

	private:
		bool orientation = true;

	public:
		void setOrientation(bool value);
		bool getOrientation();

		bool isForward() override;
	};
}