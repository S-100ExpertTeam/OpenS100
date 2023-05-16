#include "stdafx.h"
#include "GF_InformationType.h"

namespace GF
{
	InformationType::InformationType()
	{

	}

	InformationType::~InformationType()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			delete (*i);
		}

		attributes.clear();
	}

	int InformationType::GetAttributeCount()
	{
		return attributes.size();
	}
}