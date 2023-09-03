#include "stdafx.h"
#include "GM_CompositeCurve.h"

namespace GM
{
	CompositeCurve::CompositeCurve()
	{

	}

	CompositeCurve::CompositeCurve(CompositeCurve& item)
	{
		this->component = item.component;
	}

	CompositeCurve::~CompositeCurve()
	{
		
	}

	GeometryType CompositeCurve::GetType()
	{
		return GeometryType::CompositeCurve;
	}

	int CompositeCurve::getPointCount()
	{
		int result = 0;
		for (auto i = component.begin(); i != component.end(); i++) {
			result += (*i)->getPointCount();
		}
		return result;
	}

	double CompositeCurve::getX(int index)
	{
		for (int i = 0; i < component.size(); i++) {
			if (index < component.at(i)->getPointCount()) {
				return component.at(i)->getX(index);
			}
			else {
				index -= component.at(i)->getPointCount();
			}
		}

		return 0;
	}

	double CompositeCurve::getY(int index)
	{
		for (int i = 0; i < component.size(); i++) {
			if (index < component.at(i)->getPointCount()) {
				return component.at(i)->getY(index);
			}
			else {
				index -= component.at(i)->getPointCount();
			}
		}

		return 0;
	}

	void CompositeCurve::Add(std::string curveMemberID)
	{
		component.push_back(new OrientableCurve(curveMemberID));
	}

	void CompositeCurve::Add(GM::OrientableCurve* item)
	{
		component.push_back(item);
	}

	void CompositeCurve::Add(GM::Curve* item)
	{
		component.push_back(item);
	}

	void CompositeCurve::Add(GM::CompositeCurve* item)
	{
		component.push_back(item);
	}
}