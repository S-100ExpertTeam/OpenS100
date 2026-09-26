#pragma once
#include "S100_Description.h"

#include <string>
#include <vector>

namespace Portrayal
{
	class CatalogItem
	{
	public:
		CatalogItem();
		virtual ~CatalogItem();

	private:
		std::string id;
		std::vector<S100_Description*> description;

	public:
		std::string  GetId();
		std::wstring GetIdW();
		void SetId(const std::string& value);
		void SetId(const std::wstring& value);

		S100_Description* GetDescription(int index);
		std::vector<S100_Description*>* GetDescription();

		void AddDescription(S100_Description* value);
		int GetCountOfDescription();
	};
}
