#pragma once

#include "S100SpatialObject.h"

#include <string>

namespace IF
{
	class FeatureType
	{
		virtual std::string GetName(S100SpatialObject* so) {
			return "";
		};

		virtual std::wstring GetNameAsWString(S100SpatialObject* so) {
			return L"";
		};

		virtual std::string GetID() {
			return "";
		}

		virtual std::wstring GetIDAsWString() {
			return L"";
		}
	};
}