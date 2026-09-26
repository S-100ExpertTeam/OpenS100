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
		std::string defaultValue;
		std::string value;

	public:
		ParameterType GetType();
		void SetType(ParameterType value);
		void SetType(const std::string& value);
		void SetType(const std::wstring& value);

		std::string  GetDefault();
		std::wstring GetDefaultW();
		void SetDefault(const std::string& value);
		void SetDefault(const std::wstring& value);

		std::string  getValue();
		std::wstring getValueW();
		void setValue(const std::string& value);
		void setValue(const std::wstring& value);

		std::string getName();

		void GetContents(pugi::xml_node& node);
	};
}
