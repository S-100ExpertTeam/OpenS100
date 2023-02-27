#pragma once

#include "S100SpatialObject.h"

#include <string>

namespace IF
{
	class FeatureType
	{
	public:
		virtual std::string GetCode(S100SpatialObject* so) {
			return "";
		};

		virtual std::wstring GetCodeAsWString(S100SpatialObject* so) {
			return L"";
		};

		virtual std::string GetID() {
			return "";
		}

		virtual std::wstring GetIDAsWString() {
			return L"";
		}

		virtual int GetIDAsInteger() {
			return 0;
		}

		virtual bool IsNoGeometry() {
			return false;
		}

		virtual int GetFeatureRelationCount() {
			return 0;
		}

		virtual int GetInformationRelationCount() {
			return 0;
		}
	};
}