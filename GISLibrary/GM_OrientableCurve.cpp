#include "stdafx.h"
#include "GM_OrientableCurve.h"

#include <regex>

namespace GM
{
	OrientableCurve::OrientableCurve()
	{

	}

	OrientableCurve::OrientableCurve(const OrientableCurve& item) : Primitive(item)
	{
		this->orientation = item.orientation;
		this->baseCurveID = item.baseCurveID;
		if (item.baseCurve) {
			if (typeid(OrientableCurve) == typeid(*item.baseCurve)) {
				this->baseCurve = new OrientableCurve(*item.baseCurve);
			}
			else if (typeid(Curve) == typeid(*item.baseCurve)) {
				this->baseCurve = new Curve((Curve&)*item.baseCurve);
			}
			else if (typeid(CompositeCurve) == typeid(*item.baseCurve)) {
				this->baseCurve = new CompositeCurve((CompositeCurve&)*item.baseCurve);
			}
		}
	}

	OrientableCurve::OrientableCurve(std::string baseCurveID)
	{
		this->baseCurveID = baseCurveID;
	}
	
	OrientableCurve::~OrientableCurve()
	{
		delete this->baseCurve;
		this->baseCurve = nullptr;
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