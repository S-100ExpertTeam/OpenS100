#include "stdafx.h"
#include "CatalogItem.h"

#include "../LibMFCUtil/LibMFCUtil.h"

namespace Portrayal
{
	CatalogItem::CatalogItem()
	{

	}

	CatalogItem::~CatalogItem()
	{
		for (auto i = description.begin(); i != description.end(); i++)
		{
			delete *i;
		}
	}

	void CatalogItem::SetId(std::wstring& value)
	{
		id = value;
	}

	std::wstring CatalogItem::GetId()
	{
		return id;
	}

	std::string CatalogItem::GetIdAsString()
	{
		return LibMFCUtil::WStringToString(id);
	}

	void CatalogItem::AddDescription(S100_Description* value) 
	{
		description.push_back(value);
	}

	S100_Description* CatalogItem::GetDescription(int index)
	{
		return description.at(index);
	}

	std::vector<S100_Description*>* CatalogItem::GetDescription()
	{
		return &description;
	}

	int CatalogItem::GetCountOfDescription() 
	{
		return (int)description.size();
	}
}