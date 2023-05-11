#pragma once

#include "IF_InformationType.h"
#include "GeometryType.h"

#include <string>

namespace GM
{
	class Object
	{
	public:
		Object();

	public:
		virtual std::string GetID() { return ""; }
		virtual std::wstring GetIDAsWString() { return L""; }

		virtual int GetInformationRelationCount() { return 0; }
		virtual std::string GetAssociatedInformationID(int index) { return ""; }

		virtual GeometryType GetType() { return GeometryType::Object; }
	};
} 