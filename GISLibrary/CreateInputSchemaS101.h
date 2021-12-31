#pragma once

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class S101Cell;
class R_CurveRecord;
class CreateInputSchemaS101
{
public:
	CreateInputSchemaS101();
	virtual ~CreateInputSchemaS101();

public:
	static void CreatePortrayalCatalogueInputData(S101Cell* cell);
    static void SetFeaturesType(S101Cell* cell, pugi::xml_node node);
	static void SetInformationsType(S101Cell* cell, pugi::xml_node node);
	static void SetVectorPointsType(S101Cell* cell, pugi::xml_node node);
	static void SetVectorMultiPointsType(S101Cell* cell, pugi::xml_node node);
	static void SetVectorCurvesType(S101Cell* cell, pugi::xml_node node);
	static void SetVectorCompositeCurvesType(S101Cell* cell, pugi::xml_node node);
	static void SetVectorSurfacesType(S101Cell* cell, pugi::xml_node node);
	static void SetCurveElement(pugi::xml_node node, R_CurveRecord* curveRecord);
	static void CompositeCurveToCurve(S101Cell* cell,pugi::xml_node node, int RCID, int ORNT);
	static void SurfaceToCurve(S101Cell* cell, pugi::xml_node node);
};