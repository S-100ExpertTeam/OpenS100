#include "stdafx.h"
#include "GF_MaskReference.h"

namespace GF
{
	MaskReference::MaskReference()
	{

	}

	MaskReference::MaskReference(const MaskReference& other)
	{
		geometryID = other.geometryID;
		maskIndicator = other.maskIndicator;
	}

	MaskReference::~MaskReference()
	{

	}
}