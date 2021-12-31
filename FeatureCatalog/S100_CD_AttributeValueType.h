#pragma once

#include <string>

namespace FCD
{
	enum class S100_CD_AttributeValueType 
	{
		none,
		boolean,
		enumeration,
		integer,
		real,
		date,
		text,
		time,
		dateTime,
		URI,
		URL,
		URN,
		S100_CodeList,
		S100_TruncatedDate
	};

	S100_CD_AttributeValueType StringToS100_CD_AttributeValueType(std::string& value);
	S100_CD_AttributeValueType StringToS100_CD_AttributeValueType(std::wstring& value);

	std::string S100_CD_AttributeValueTypeToString(S100_CD_AttributeValueType value);
	std::wstring S100_CD_AttributeValueTypeToWString(S100_CD_AttributeValueType value);
}