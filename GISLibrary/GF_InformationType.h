#pragma once

#include "GF_ObjectType.h"
#include "GF_ThematicAttributeType.h"

#include <vector>

namespace GF
{
	class InformationType : 
		public ObjectType
	{
	public:
		InformationType();
		~InformationType();

	public:
		//std::vector<ThematicAttributeType*> attributes;

	public:
		//virtual int GetAttributeCount();

		//virtual ThematicAttributeType* GetAttribute(int index);

		//virtual std::string GetAttributeValue(int index);

		//virtual int GetParentAttributeIndex(int index);
	};
}