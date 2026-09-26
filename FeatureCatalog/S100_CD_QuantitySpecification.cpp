#include "stdafx.h"
#include "S100_CD_QuantitySpecification.h"

#include <algorithm>

#include <pugixml.hpp>

namespace FCD
{
	S100_CD_QuantitySpecification StringToS100_CD_QuantitySpecification(std::string& value)
	{
		int maxIndex = (int)S100_CD_QuantitySpecification::otherQuantity + 1;
		for (int i = 0; i < maxIndex; i++)
		{
			if (0 == value.compare(S100_CD_QuantitySpecification_String[i]))
			{
				return (S100_CD_QuantitySpecification)i;
			}
		}

		return S100_CD_QuantitySpecification::none;
	}

	std::string S100_CD_QuantitySpecificationToString(S100_CD_QuantitySpecification value)
	{
		auto index = (int)value;
		if (index < (int)S100_CD_QuantitySpecification::none|| 
			index > (int)S100_CD_QuantitySpecification::otherQuantity)
		{
			return "";
		}

		return S100_CD_QuantitySpecification_String[(int)value];
	}

	std::string S100_CD_QuantitySpecificationToWString(S100_CD_QuantitySpecification value)
	{
		auto str = S100_CD_QuantitySpecificationToString(value);
		return str;
	}
}