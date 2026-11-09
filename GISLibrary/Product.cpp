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
	auto _fc = new FeatureCatalogue();
	if (_fc->Read(path)) {
		if (this->fc.find(_fc->GetVersionNumberW()) == this->fc.end()) {
			this->fc.insert({ _fc->GetVersionNumberW(), _fc });
			return true;
		}
	}

	delete _fc;
	return false;
}

bool Product::OpenPC(std::wstring path)
{
	auto _pc = new PortrayalCatalogue();
	if (_pc->Open(path)) {

		if (this->pc.find(_pc->GetVersionW()) == this->pc.end()) {
			this->pc.insert({ _pc->GetVersionW(), _pc });
			return true;
		}
	}

	delete _pc;
	return false;
}