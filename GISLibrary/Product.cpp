#include "stdafx.h"
#include "Product.h"

#include "../FeatureCatalog/FeatureCatalogue.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

Product::Product()
{

}

Product::~Product()
{
	// Release fc
	for (auto i = fc.begin(); i != fc.end(); i++)
	{
		delete i->second;
	}

	fc.clear();

	// Release pc
	for (auto i = pc.begin(); i != pc.end(); i++)
	{
		delete i->second;
	}

	pc.clear();
}

bool Product::OpenFC(std::wstring path)
{
	auto fc = new FeatureCatalogue();
	if (fc->Read(path)) {

		if (this->fc.find(fc->GetVersionNumber()) == this->fc.end()) {
			this->fc.insert({ fc->GetVersionNumber(), fc });
			return true;
		}
	}

	delete fc;
	return false;
}

bool Product::OpenPC(std::wstring path)
{
	auto pc = new PortrayalCatalogue();
	if (pc->Open(path)) {

		if (this->pc.find(pc->GetVersion()) == this->pc.end()) {
			this->pc.insert({ pc->GetVersion(), pc });
			return true;
		}
	}

	delete pc;
	return false;
}