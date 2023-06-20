#include "stdafx.h"
#include "GM_OrientableCurve.h"

#include <regex>

namespace GM
{
	OrientableCurve::OrientableCurve()
	{

	}

	OrientableCurve::OrientableCurve(std::string baseCurveID)
	{
		this->baseCurveID = baseCurveID;
	}
	
	OrientableCurve::~OrientableCurve()
	{

	}

	GeometryType OrientableCurve::GetType()
	{
		return GeometryType::OrientableCurve;
	}

	int OrientableCurve::GetBaseCurveIDAsInt()
	{
		std::string digitID = std::regex_replace(baseCurveID, std::regex(R"([\D])"), "");
		return std::stoi(digitID);
	}
}