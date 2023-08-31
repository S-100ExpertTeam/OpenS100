#include "stdafx.h"
#include "Inventory.h"
#include "S100Utilities.h"

Inventory::Inventory()
{

}

Inventory::Inventory(Inventory& item)
{
	this->geoPolygon = item.geoPolygon;
	this->scaleBands = item.scaleBands;
}

Inventory::~Inventory()
{

}

void Inventory::setScaleBands(DataCoverage dataCoverage)
{
	scaleBands = S100Utilities::GetScaleBands(dataCoverage);
}