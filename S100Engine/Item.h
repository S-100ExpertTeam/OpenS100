#pragma once
#include "SRGB.h"

#include <string>

namespace libS100Engine
{

	class Item
	{
	public:
		Item();
		virtual ~Item();

	public:
		std::string token;
		SRGB    srgb;
	};
}