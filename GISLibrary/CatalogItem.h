#pragma once
#include <atlstr.h>

class FeatureCatalogue;
class CatalogItem
{
public:
	CatalogItem();
	virtual ~CatalogItem();
	
public:
	FeatureCatalogue* m_catalog;
	CString m_name;
	CString m_version;
};