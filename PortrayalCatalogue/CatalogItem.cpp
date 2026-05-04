#include "stdafx.h"
#include "CatalogItem.h"

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

	void CatalogItem::SetId(const std::string& value)  { id = value; }
	void CatalogItem::SetId(const std::wstring& value) { id = toUtf8(value); }
	std::string  CatalogItem::GetId()  { return id; }
	std::wstring CatalogItem::GetIdW() { return toWide(id); }

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
