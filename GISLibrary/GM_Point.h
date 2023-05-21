#pragma once

#include "GM_Primitive.h"
#include "DirectPosition.h"
#include "PugixmlElement.h"

#include <string>

namespace GM
{
	class Point : 
		public Primitive,
		public PugixmlElement
	{
	public:
		Point();
		virtual ~Point();

	public:
		DirectPosition position;

	public:
		void Read(pugi::xml_node& node);
		void Write(pugi::xml_node& node);
		GeometryType GetType() override;
	};
}