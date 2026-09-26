#include"stdafx.h"
#include "Enum_GeometricPrimitive.h"

namespace PCD 
{
	GeometricPrimitive StringToGeometricPrimitive(std::string& value)
	{
		int maxindex = (int)GeometricPrimitive::complex + 1;
		for (int i=0; i<maxindex; i++) 
		{
			if (0== value.compare(GeometricPrimitive_String[i]))
			{
				return (GeometricPrimitive)i;
			}

		}
		return GeometricPrimitive::none;
	}

	GeometricPrimitive StringToGeometricPrimitive(std::wstring& value)
	{
		std::string str = toUtf8(value);
		return StringToGeometricPrimitive(str);
	}

	std::string GeometricPrimitiveToString(GeometricPrimitive value) 
	{
		auto index = (int)value;
		if (index < (int)GeometricPrimitive::none ||
			index >(int)GeometricPrimitive::complex)
		{
			return "";
		}

		return GeometricPrimitive_String[(int)value];

	}
	std::wstring GeometricPrimitiveToWString(GeometricPrimitive value)
	{
		return toWide(GeometricPrimitiveToString(value));
	}
}