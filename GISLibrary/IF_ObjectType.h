#pragma once

#include <string>

namespace S100Interface
{
	class ObjectType
	{
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
			return 0;
		}

		virtual std::string GetAttributeValue(int index) {
			return "";
		}

		virtual int GetParentAttributeIndex(int index) {
			return 0;
		}
	};
}