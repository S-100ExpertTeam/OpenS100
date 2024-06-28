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
		Object(const Object& other);
		Object(std::string id);
		virtual ~Object();

	public:
		std::string id;
		std::string srsName;

	public:
		virtual Object operator=(const Object& other);

	public:
		std::string GetID();
		std::wstring GetIDAsWString();
		int GetIDAsInt();
		void SetID(std::string value);
		void SetID(std::wstring value);
		void SetID(int value);

		std::string getSRSName();
		void setSRSName(std::string value);

		void setParentIdSrsName(std::string id, std::string srsName);

		void readID(pugi::xml_node& node);
		void readSRSName(pugi::xml_node& node);
		void readIdSRSName(pugi::xml_node& node);
	
	public:
		virtual int GetInformationRelationCount() { return 0; }
		virtual std::string GetAssociatedInformationID(int index) { return ""; }

		virtual GeometryType GetType() { return GeometryType::Object; }

		virtual std::string GeometricPrimitiveAsString() { return "None"; }
		virtual bool isForward() { return true; }
	};
} 