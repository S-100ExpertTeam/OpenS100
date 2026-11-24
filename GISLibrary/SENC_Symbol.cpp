#include "stdafx.h"
#include "SENC_Symbol.h"

#include <pugixml.hpp>
#include "SENC_Define.h"

SENC_Symbol::SENC_Symbol()
{
	rotationCRS = PORTRAYAL_CRS;
}

SENC_Symbol::~SENC_Symbol()
{
	delete linePlacement;
	delete areaPlacement;
}

const std::string& SENC_Symbol::ReferenceUtf8()
{
	if (!referenceRead || referenceConverted != reference)
	{
		referenceUtf8 = pugi::as_utf8(reference);
		referenceConverted = reference;
		referenceRead = true;
	}
	return referenceUtf8;
}
