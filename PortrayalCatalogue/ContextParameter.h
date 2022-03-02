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
		std::wstring default;

	public:
		ParameterType* GetType();
		void SetType(ParameterType* value);
		std::wstring GetDefault();
		void SetDefault(std::wstring& value);
	};
}