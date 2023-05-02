#include "stdafx.h"
#include "S10XGML.h"

S10XGML::S10XGML()
{

}

S10XGML::~S10XGML()
{
	for (auto i = features.begin(); i != features.end(); i++)
	{
		delete (*i);
	}

	for (auto i = informations.begin(); i != informations.end(); i++)
	{
		delete (*i);
	}
}

bool S10XGML::Open(CString _filepath)
{


	return true;
}