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

	std::string OrientableCurve::GeometricPrimitiveAsString()
	{
		return "Curve";
	}

	std::string OrientableCurve::GetBaseCurveID()
	{
		return baseCurveID;
	}

	int OrientableCurve::GetBaseCurveIDAsInt()
	{
		std::string digitID = std::regex_replace(baseCurveID, std::regex(R"([\D])"), "");
		return std::stoi(digitID);
	}

	void OrientableCurve::Read(pugi::xml_node& node)
	{
		
	}

	OrientableCurve* OrientableCurve::getBaseCurve()
	{
		return baseCurve;
	}

	void OrientableCurve::setBaseCurve(OrientableCurve* baseCurve)
	{
		if (this->baseCurve == nullptr) {
			this->baseCurve = baseCurve;
		}
	}

	int OrientableCurve::getPointCount()
	{
		if (baseCurve) {
			return baseCurve->getPointCount();
		}

		return 0;
	}
		
	double OrientableCurve::getX(int index)
	{
		if (baseCurve) {
			if (orientation) {
				return baseCurve->getX(index);
			}
			else {
				return baseCurve->getX(baseCurve->getPointCount() - index - 1);
			}
		}

		return 0;
	}

	double OrientableCurve::getY(int index)
	{
		if (baseCurve) {
			if (orientation) {
				return baseCurve->getY(index);
			}
			else {
				return baseCurve->getY(baseCurve->getPointCount() - index - 1);
			}
		}

		return 0;
	}
}