#include "stdafx.h"
#include "S100Layer.h"

#include "../LatLonUtility/cpp_util.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <sstream>

S100Layer::S100Layer() : Layer()
{

}

S100Layer::S100Layer(int productNumber) : Layer()
{
	SetProductNumber(productNumber);
}

S100Layer::S100Layer(int productNumber, FeatureCatalogue* fc, PortrayalCatalogue* pc) : Layer()
{
	SetFeatureCatalog(fc);

	if (pc) {
		SetPC(pc);
	}

	SetProductNumber(productNumber);
}

S100Layer::S100Layer(FeatureCatalogue* fc, PortrayalCatalogue *pc) : Layer()
{	
	SetFeatureCatalog(fc);
	SetPC(pc);
	SetProductNumber(fc->GetProductIdW());
}

S100Layer::~S100Layer()
{
	DeleteCatalog();
}

void S100Layer::SetFeatureCatalog(FeatureCatalogue* value)
{
	featureCatalogue = value;
}

FeatureCatalogue* S100Layer::GetFeatureCatalog()
{
	return featureCatalogue;
}

FeatureCatalogue* S100Layer::GetFC()
{
	return GetFeatureCatalog();
}

void S100Layer::SetPC(PortrayalCatalogue *value)
{
	portrayalCatalogue = value;
}

PortrayalCatalogue* S100Layer::GetPC()
{
	return portrayalCatalogue;
}

bool S100Layer::OpenFC(const std::wstring& path)
{
	FeatureCatalogue* fc = new FeatureCatalogue();
	fc->Read(path);
	
	if (fc) {
		SetIndividualFC(true);
		SetFeatureCatalog(fc);
		return true;
	}
		
	SetIndividualFC(false);
	delete fc;
	return false;
}

void S100Layer::SetIndividualFC(bool value) 
{
	hasIndividualFC = value;
}

bool S100Layer::GetIndividualFC() 
{
	return hasIndividualFC;
}

void S100Layer::SetIndividualPC(bool value) 
{
	hasIndividualPC = value;
}
bool S100Layer::GetIndividualPC() 
{
	return hasIndividualPC;
}

void S100Layer::DeleteCatalog()
{
	if (GetIndividualFC())
	{
		delete	featureCatalogue;
		featureCatalogue = nullptr;
	}

	if (GetIndividualPC())
	{
		delete portrayalCatalogue;
		portrayalCatalogue = nullptr;
	}
}

S100SpatialObject* S100Layer::GetS100SpatialObject()
{
	return (S100SpatialObject*)m_spatialObject;
}

int S100Layer::GetProductNumber()
{
	return productNumber;
}

void S100Layer::SetProductNumber(int value)
{
	productNumber = value;
}

void S100Layer::SetProductNumber(std::wstring value)
{
	// tokenize value by "-"
	std::vector<std::wstring> tokens;

	std::wstringstream ss(value);
	std::wstring token;
	while (std::getline(ss, token, L'-'))
	{
		tokens.push_back(token);
	}

	if (tokens.size() == 2)
	{
		productNumber = cpp_util::stoi(tokens.at(1));
	}
}

void S100Layer::InitDraw()
{
	drawingPriority.clear();
	drawingSet.Init();
}
