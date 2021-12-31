#include "stdafx.h"
#include "Catalogue.h"

Catalogue::Catalogue()
{

}

Catalogue::~Catalogue()
{

}

void Catalogue::Set_Catalogue(FeatureCatalogue _Featurecatalog, PortrayalCatalogue _portrayalCatalogue)
{
	Featurecatalog = _Featurecatalog;
	portrayalCatalogue = _portrayalCatalogue;
}