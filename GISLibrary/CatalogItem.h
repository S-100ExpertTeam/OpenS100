#pragma once

class FeatureCatalogue;

class CatalogItem
{
public:
	CatalogItem();
	virtual ~CatalogItem();
	
public:
	FeatureCatalogue* fc = nullptr;
	CString m_name;
	CString m_version;
};