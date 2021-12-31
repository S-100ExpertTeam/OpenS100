#include "stdafx.h" 
#include "S100_CD_AttributeValueType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace FCD
{

	S100_CD_AttributeValueType StringToS100_CD_AttributeValueType(std::string& value)
	{
		if (value.compare("boolean") == 0)
		{
			return S100_CD_AttributeValueType::boolean;
		}
		else if (value.compare("enumeration") == 0)
		{
			return S100_CD_AttributeValueType::enumeration;
		}
		else if (value.compare("integer") == 0)
		{
			return S100_CD_AttributeValueType::integer;
		}
		else if (value.compare("real") == 0)
		{
			return S100_CD_AttributeValueType::real;
		}
		else if (value.compare("date") == 0)
		{
			return S100_CD_AttributeValueType::date;
		}
		else if (value.compare("text") == 0)
		{
			return S100_CD_AttributeValueType::text;
		}
		else if (value.compare("time") == 0)
		{
			return S100_CD_AttributeValueType::time;
		}
		else if (value.compare("dateTime") == 0)
		{
			return S100_CD_AttributeValueType::dateTime;
		}
		else if (value.compare("URI") == 0)
		{
			return S100_CD_AttributeValueType::URI;
		}
		else if (value.compare("URL") == 0)
		{
			return S100_CD_AttributeValueType::URL;
		}
		else if (value.compare("URN") == 0)
		{
			return S100_CD_AttributeValueType::URN;
		}
		else if (value.compare("S100_CodeList") == 0)
		{
			return S100_CD_AttributeValueType::S100_CodeList;
		}
		else if (value.compare("S100_TruncatedDate") == 0)
		{
			return S100_CD_AttributeValueType::S100_TruncatedDate;
		}

		return S100_CD_AttributeValueType::none;
	}

	S100_CD_AttributeValueType StringToS100_CD_AttributeValueType(std::wstring& value)
	{
		std::string str = pugi::as_utf8(value);
		return StringToS100_CD_AttributeValueType(str);
	}

	std::string S100_CD_AttributeValueTypeToString(S100_CD_AttributeValueType value)
	{
		if (value == S100_CD_AttributeValueType::boolean)
		{
			return "boolean";
		}
		else if (value == S100_CD_AttributeValueType::enumeration)
		{
			return "enumeration";
		}
		else if (value == S100_CD_AttributeValueType::integer)
		{
			return "integer";
		}
		else if (value == S100_CD_AttributeValueType::real)
		{
			return "real";
		}
		else if (value == S100_CD_AttributeValueType::date)
		{
			return "date";
		}
		else if (value == S100_CD_AttributeValueType::text)
		{
			return "text";
		}
		else if (value == S100_CD_AttributeValueType::time)
		{
			return "time";
		}
		else if (value == S100_CD_AttributeValueType::dateTime)
		{
			return "dateTime";
		}
		else if (value == S100_CD_AttributeValueType::URI)
		{
			return "URI";
		}
		else if (value == S100_CD_AttributeValueType::URL)
		{
			return "URL";
		}
		else if (value == S100_CD_AttributeValueType::URN)
		{
			return "URN";
		}
		else if (value == S100_CD_AttributeValueType::S100_CodeList)
		{
			return "S100_CodeList";
		}
		else if (value == S100_CD_AttributeValueType::S100_TruncatedDate)
		{
			return "S100_TruncatedDate";
		}

		return "";
	}

	std::wstring S100_CD_AttributeValueTypeToWString(S100_CD_AttributeValueType value)
	{
		auto str = S100_CD_AttributeValueTypeToString(value);
		return pugi::as_wide(str);
	}
}