#pragma once
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"

class Catalogue
{
public:
	Catalogue();
	virtual ~Catalogue();

public:
	FeatureCatalogue Featurecatalog;
	PortrayalCatalogue portrayalCatalogue;

public:
	void Set_Catalogue(FeatureCatalogue _Featurecatalog, PortrayalCatalogue _portrayalCatalogue);
};