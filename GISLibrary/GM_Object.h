#pragma once

#include "GF_InformationType.h"
#include "GeometryType.h"

#include "../GeoMetryLibrary/MBR.h"

#include <string>

namespace GM
{
	class Object
	{
	public:
		Object();
		Object(std::string id);
		Object(const Object& item);
		virtual ~Object();

	public:
		std::string id;

	public:
		std::string GetID();
		std::wstring GetIDAsWString();
		int GetIDAsInt();
		void SetID(std::string value);
		void SetID(std::wstring value);
		void SetID(int value);

		virtual int GetInformationRelationCount() { return 0; }
		virtual std::string GetAssociatedInformationID(int index) { return ""; }

		virtual GeometryType GetType() { return GeometryType::Object; }

		virtual std::string GeometricPrimitiveAsString() { return "None"; }
	};
} 