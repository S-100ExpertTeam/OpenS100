#pragma once

#include "IF_Attribute.h"

#include <string>
#include <vector>

namespace S100Interface
{
	class ObjectType
	{
	public:
		ObjectType();
		virtual ~ObjectType();

	public:
		std::vector<Attribute*> attributes;

	public:
		virtual std::string GetID() {
			return "";
		}

		virtual std::wstring GetIDAsWString() {
			return L"";
		}

		virtual int GetIDAsInteger() {
			return 0;
		}

		virtual int GetInformationRelationCount() {
			return 0;
		}

		virtual std::string GetAssociatedInformationID(int index) {
			return "";
		}

		virtual int GetAttributeCount() {
			return (int)attributes.size();
		}

		virtual Attribute* GetAttribute(int index);

		virtual std::string GetAttributeValue(int index) {
			return "";
		}

		virtual int GetParentAttributeIndex(int index) {
			return 0;
		}
	};
}