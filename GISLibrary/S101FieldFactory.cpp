#include "stdafx.h"
#include "S101Creator.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "F_SPAS.h"
#include "F_FASC.h"
#include "F_MASK.h"

F_ATTR* S101FieldFactory::createAttribute()
{
	F_ATTR* attr = new (std::nothrow) F_ATTR();
	return attr;
}

void S101FieldFactory::destoryAttribute(F_ATTR* attr)
{
	if (attr)
	{
		delete attr;
		attr = nullptr;
	}
}

F_INAS* S101FieldFactory::createInformationAssociation()
{
	F_INAS* inas = new (std::nothrow) F_INAS();
	return inas;
}

void S101FieldFactory::destoryInformationAssociation(F_INAS* inas)
{
	if (inas)
	{
		delete inas;
		inas = nullptr;
	}
}

F_SPAS* S101FieldFactory::createSpatialAssociation()
{
	F_SPAS* spas = new (std::nothrow) F_SPAS();
	return spas;
}

void S101FieldFactory::destorySpatialAssociation(F_SPAS* spas)
{
	if (spas)
	{
		delete spas;
		spas = nullptr;
	}
}

F_FASC* S101FieldFactory::createFeatureAssociation()
{
	F_FASC* fasc = new (std::nothrow) F_FASC();
	return fasc;
}

void S101FieldFactory::destoryFeatureAssociation(F_FASC* fasc)
{
	if (fasc)
	{
		delete fasc;
		fasc = nullptr;
	}
}

F_MASK* S101FieldFactory::createMaskedSpatialType()
{
	F_MASK* mask = new (std::nothrow) F_MASK();
	return mask;
}

void S101FieldFactory::destoryMaskedSpatialType(F_MASK* mask)
{
	if (mask)
	{
		delete mask;
		mask = nullptr;
	}
}

