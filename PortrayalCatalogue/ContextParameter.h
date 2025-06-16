#pragma once
#include "CatalogItem.h"
#include "Enum_ParameterType.h"
#include "S100_Description.h"

namespace Portrayal
{
	
	class ContextParameter :
		public CatalogItem
	{
	public:
		ContextParameter();
		virtual ~ContextParameter();

	private:
		ParameterType type;
		std::wstring defaultValue;
		std::wstring value;

	public:
		ParameterType GetType();
		void SetType(ParameterType value);
		void SetType(std::wstring value);

		std::wstring GetDefault();
		std::string GetDefaultAsString();
		void SetDefault(std::wstring& value);

		std::wstring getValue();
		std::string getValueAsString();
		void setValue(std::wstring& value);

		std::string getName();

		void GetContents(pugi::xml_node& node);
	};
}