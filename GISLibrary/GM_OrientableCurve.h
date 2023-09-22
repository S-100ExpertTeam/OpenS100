#pragma once

#include "GM_Primitive.h"

#include "DirectPosition.h"

#include <string>

namespace GM
{
	class OrientableCurve : public Primitive
	{
	public:
		OrientableCurve();
		OrientableCurve(const OrientableCurve& item);
		OrientableCurve(std::string baseCurveID);
		virtual ~OrientableCurve();

	public:
		// true : +
		// false : -
		bool orientation = false; 
		std::string baseCurveID;

	private:
		OrientableCurve* baseCurve = nullptr; // released by dataset, not used

	public:
		virtual int getPointCount();
		virtual double getX(int index);
		virtual double getY(int index);

	public:
		GeometryType GetType() override;
		std::string GeometricPrimitiveAsString() override;

		std::string GetBaseCurveID();
		int GetBaseCurveIDAsInt();

		void Read(pugi::xml_node& node);

		OrientableCurve* getBaseCurve();
		void setBaseCurve(OrientableCurve* baseCurve);
	};
}