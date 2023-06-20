#include "stdafx.h"
#include "CatalogManager.h"

CatalogManager::CatalogManager()
{

}

CatalogManager::~CatalogManager()
{
	for (auto i = FCs.begin(); i != FCs.end(); i++)
	{
		delete (*i);
	}

	for (auto i = PCs.begin(); i != PCs.end(); i++)
	{
		delete (*i);
	}
}