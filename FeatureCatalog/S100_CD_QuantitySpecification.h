#pragma once

#include <string>

// Feature Concept Dictionary
namespace FCD
{
	enum class S100_CD_QuantitySpecification
	{
		none = 0,
		angularVelocity = 1,
		area = 2,
		density = 3,
		duration = 4,
		frequency = 5,
		length = 6,
		mass = 7,
		planeAngle = 8,
		power = 9,
		pressure = 10,
		salinity = 11,
		speed = 12,
		temperature = 13,
		volume = 14,
		weight = 15,
		otherQuantity = 16
	};

	static const char* S100_CD_QuantitySpecification_String[] =
	{
		"none",
		"angularVelocity",
		"area",
		"density",
		"duration",
		"frequency",
		"length",
		"mass",
		"planeAngle",
		"power",
		"pressure",
		"salinity",
		"speed",
		"temperature",
		"volume",
		"weight",
		"otherQuantity"
	};

	S100_CD_QuantitySpecification StringToS100_CD_QuantitySpecification(std::string& value);
	S100_CD_QuantitySpecification StringToS100_CD_QuantitySpecification(std::wstring& value);

	std::string S100_CD_QuantitySpecificationToString(S100_CD_QuantitySpecification value);
	std::wstring S100_CD_QuantitySpecificationToWString(S100_CD_QuantitySpecification value);
}