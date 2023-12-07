#include "stdafx.h"
#include "GISLibrary.h"
#include "S10XGML.h"
#include "GM_Point.h"
#include "GM_MultiPoint.h"
#include "GM_Curve.h"
#include "GM_OrientableCurve.h"
#include "GM_CompositeCurve.h"
#include "GM_Surface.h"
#include "GF_FeatureType.h"
#include "GF_InformationType.h"

#include "../LatLonUtility/LatLonUtility.h"

S10XGML::S10XGML(D2D1Resources* d2d1) : S100SpatialObject(d2d1)
{
	type = S100SpatialObjectType::S10XGML;
	m_FileType = S100_FileType::FILE_S_100_VECTOR;
}

S10XGML::~S10XGML()
{
	for (auto i = features.begin(); i != features.end(); i++)
	{
		delete (*i);
	}

	for (auto i = informations.begin(); i != informations.end(); i++)
	{
		delete (*i);
	}

	for (auto i = geometries.begin(); i != geometries.end(); i++)
	{
		auto geometry = (*i);
		delete geometry;
		geometry = nullptr;
	}
}

bool S10XGML::Open(CString _filepath)
{
	SetFilePath(_filepath);

	std::wstring path = _filepath;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str()); 
	pugi::xml_node root = doc.first_child();

	auto child = root.first_child();
	while (child)
	{
		std::string childName = child.name();

		if (strcmp(child.name(), "gml:boundedBy") == 0)
		{
			envelop.Read(child);
		}
		else if (childName.find("DatasetIdentificationInformation") != std::string::npos)
		{
			datasetIdentificationInformation.Read(child);
		}
		else if (strcmp(child.name(), "members") == 0)
		{
			ReadMembers(child);
		}
		else if (strcmp(child.name(), "S100:Point") == 0)
		{
			auto point = ReadPoint(child);
			if (point) {
				AddGeometry(point);
			}
		}
		else if (strcmp(child.name(), "S100:MultiPoint") == 0)
		{
			auto multiPoint = ReadMultiPoint(child);
			if (multiPoint) {
				AddGeometry(multiPoint);
			}
		}
		else if (strcmp(child.name(), "S100:Curve") == 0)
		{
			auto curve = ReadCurve(child);
			if (curve) {
				AddGeometry(curve);
			}
		}	
		else if (strcmp(child.name(), "S100:OrientableCurve") == 0)
		{
			auto orientableCurve = ReadOrientableCurve(child);
			if (orientableCurve) {
				AddGeometry(orientableCurve);
			}
		}
		else if (strcmp(child.name(), "S100:CompositeCurve") == 0)
		{
			auto compositeCurve = ReadCompositeCurve(child);
			if (compositeCurve) {
				AddGeometry(compositeCurve);
			}
		}
		else if (strcmp(child.name(), "S100:Surface") == 0)
		{
			auto surface = ReadSurface(child);
			if (surface) {
				AddGeometry(surface);
			}
		}
		else if (strcmp(child.name(), "imember") == 0)
		{
			ReadIMember(child);
		}
		else if (strcmp(child.name(), "member") == 0)
		{
			ReadMember(child);
		}

		child = child.next_sibling();
	}

	//SetGeometry();
	//CalcMBR();

	return true;
}

bool S10XGML::SaveToInputXML(std::string path)
{
	auto xsd = "S" + std::to_string(GetProductNumber()) + "DataModel.xsd";

	pugi::xml_document doc;

	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";

	auto root = doc.append_child("Dataset");
	root.append_attribute("xmlns:xsi").set_value("http://www.w3.org/2001/XMLSchema-instance");
	root.append_attribute("xmlns:S100").set_value("http://www.iho.int/S100BaseModel");
	root.append_attribute("xmlns:noNamespaceSchemaLocation").set_value(xsd.c_str());

	auto informationTypes = root.append_child("InformationTypes");
	auto featureTypes = root.append_child("Features");

	WriteInputXML_InformationTypes(informationTypes);
	WriteInputXML_FeatureTypes(featureTypes);

	doc.save_file(path.c_str());

	return true;
}

GM::Object* S10XGML::GetGeometry(std::string id)
{
	for (auto i = geometries.begin(); i != geometries.end(); i++) {
		auto geometry = (*i);
		if (!geometry->GetID().compare(id)) {
			return geometry;
		}
	}

	return nullptr;
}

int S10XGML::GetFeatureCount()
{
	return (int)features.size();
}

int S10XGML::GetInformationCount()
{
	return (int)informations.size();
}

std::wstring S10XGML::GetFeatureTypeCodeByID(std::wstring id)
{
	return GetFeatureTypeCodeByID(pugi::as_utf8(id));
}

std::wstring S10XGML::GetFeatureTypeCodeByID(std::string id)
{
	auto featureType = GetFeatureType(id);
	if (featureType) {
		return pugi::as_wide(featureType->GetCode());
	}

	return std::wstring(L"");
}

std::wstring S10XGML::GetFeatureTypeCodeByID(int id)
{
	return GetFeatureTypeCodeByID(std::to_string(id));
}

std::wstring S10XGML::GetInformationTypeCodeByID(std::wstring id)
{
	return GetInformationTypeCodeByID(pugi::as_utf8(id));
}

std::wstring S10XGML::GetInformationTypeCodeByID(std::string id)
{
	auto informationType = GetInformationType(id);
	if (informationType) {
		return pugi::as_wide(informationType->GetCode());
	}

	return std::wstring(L"");
}

std::wstring S10XGML::GetInformationTypeCodeByID(int id)
{
	return GetInformationTypeCodeByID(std::to_string(id));
}

GM::OrientableCurve* S10XGML::GetOrientableCurve(std::string id)
{
	auto geometry = GetGeometry(id);

	if (geometry) {
		auto type = geometry->GetType();
		if (type == GM::GeometryType::OrientableCurve) {
			return (GM::OrientableCurve*)geometry;
		}
		else if (type == GM::GeometryType::Curve) {
			return (GM::Curve*)geometry;
		}
		else if (type == GM::GeometryType::CompositeCurve) {
			return (GM::CompositeCurve*)geometry;
		}
	}

	return nullptr;
}

bool S10XGML::ReadMembers(pugi::xml_node& node)
{
	auto fc = GetFC();

	auto child = node.first_child();
	while (child)
	{
		std::string code = child.name();
		auto feature = fc->GetFeatureType(code);
		if (feature)
		{
			ReadFeature(child, fc);
		}
		else
		{
			auto info = fc->GetInformationType(code);
			if (info)
			{
				ReadInformation(child, fc);
			}
		}

		child = child.next_sibling();
	}

	return true;
}

GF::FeatureType* S10XGML::ReadFeature(pugi::xml_node& node, FeatureCatalogue* fc)
{
	auto feature = new GF::FeatureType();

	feature->id = node.attribute("gml:id").value();
	feature->code = node.name();
	feature->code = DeleteXMLNamespace(feature->code);

	features.push_back(feature);

	auto child = node.first_child();
	while (child) {
		std::string childName = child.name();

		auto attribute = fc->GetAttribute(childName);
		auto role = fc->GetRole(childName);

		if (attribute) {
			ReadObjectAttribute(child, feature, fc);
		}
		else if (childName.compare("geometry") == 0) {
			ReadFeatureGeometry(child, feature);
		}
		else {// if (role) {
			ReadFeatureRole(child, feature, fc);
		}

		child = child.next_sibling();
	}
	return feature;
}

GF::InformationType* S10XGML::ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc)
{
	auto information = new GF::InformationType();

	information->id = node.attribute("gml:id").value();
	information->code = node.name();
	information->code = DeleteXMLNamespace(information->code);

	informations.push_back(information);

	auto child = node.first_child();
	while (child)
	{
		std::string childName = child.name();

		auto attribute = fc->GetAttribute(childName);
		if (attribute)
		{
			ReadObjectAttribute(child, information, fc);
		}
		else
		{
			ReadInformationRole(child, information, fc);
		}

		child = child.next_sibling();
	}
	return information;
}

GM::Point* S10XGML::ReadPoint(pugi::xml_node& node, std::string id, std::string srsName)
{
	auto object = new GM::Point();
	object->setParentIdSrsName(id, srsName);
	object->readIdSRSName(node);

	auto strPos = node.child_value("gml:pos");
		
	auto strPosList = LatLonUtility::Split(strPos, " ");

	if (strPosList.size() != 2)
	{
		delete object;
		return false;
	}

	int latIndex = 1;
	int lonIndex = 0;

	if (object->getSRSName().find("4326") != std::string::npos)
	{
		latIndex = 0;
		lonIndex = 1;
	}

	double lat = std::stod(strPosList.at(latIndex));
	double lon = std::stod(strPosList.at(lonIndex));

	object->position.SetX(lon);
	object->position.SetY(lat);

	return object;
}

GM::MultiPoint* S10XGML::ReadMultiPoint(pugi::xml_node& node, std::string id, std::string srsName)
{
	auto object = new GM::MultiPoint();
	object->setParentIdSrsName(id, srsName);
	object->readIdSRSName(node);

	int latIndex = 1;
	int lonIndex = 0;

	if (object->getSRSName().find("4326") != std::string::npos)
	{
		latIndex = 0;
		lonIndex = 1;
	}

	auto nodePoint = node.child("gml:pointMembers").child("gml:Point");

	while (nodePoint)
	{
		auto strPos = nodePoint.child_value("gml:pos");

		auto strPosList = LatLonUtility::Split(strPos, " ");

		if (strPosList.size() == 3)
		{
			double lat = std::stod(strPosList.at(latIndex));
			double lon = std::stod(strPosList.at(lonIndex));
			double depth = std::stod(strPosList.at(2));
			
			object->Add(lon, lat, depth);
		}

		nodePoint = nodePoint.next_sibling();
	}

	return object;
}

GM::Curve* S10XGML::ReadCurve(pugi::xml_node& node, std::string id, std::string srsName)
{
	auto object = new GM::Curve();
	object->setParentIdSrsName(id, srsName);
	object->readIdSRSName(node);

	int latIndex = 1;
	int lonIndex = 0;

	if (object->getSRSName().find("4326") != std::string::npos)
	{
		latIndex = 0;
		lonIndex = 1;
	}

	auto strPos = node.child("gml:segments").child("gml:LineStringSegment").child_value("gml:posList");

	auto strPosList = LatLonUtility::Split(strPos, " ");
	int posCnt = (int)strPosList.size();

	if (posCnt < 4 && posCnt % 2 != 0)
	{
		delete object;
		return false;
	}

	for (int i = 0; i < posCnt; i += 2)
	{
		double lon = std::stod(strPosList.at(i + lonIndex));
		double lat = std::stod(strPosList.at(i + latIndex));
		object->Add(lon, lat);
	}

	return object;
}

GM::Curve* S10XGML::ReadLinearRing(pugi::xml_node& node)
{
	auto object = new GM::Curve();
	//object->setParentIdSrsName

	auto strPos = node.child_value("gml:posList");

	auto strPosList = LatLonUtility::Split(strPos, " ");
	int posCnt = (int)strPosList.size();

	if (posCnt < 4 && posCnt % 2 != 0)
	{
		return false;
	}
	
	//object->SetID(gmlID);

	for (int i = 0; i < posCnt; i += 2)
	{
		double lon = std::stod(strPosList.at(i));
		double lat = std::stod(strPosList.at(i + 1));
		object->Add(lon, lat);
	}

	return object;
}

GM::OrientableCurve* S10XGML::ReadOrientableCurve(pugi::xml_node& node, std::string id, std::string srsName)
{
	auto gmlid = node.attribute("gml:id").value();
	auto orientation = node.attribute("orientation").value();
	bool bOrientation = true;
	if (!strcmp(orientation, "-")) {
		bOrientation = false;
	}

	auto node_baseCurve = node.child("gml:baseCurve");
	auto attr_baseCurve_href = node_baseCurve.attribute("xlink:href");

	if (attr_baseCurve_href) {
		std::string baseCurveID = attr_baseCurve_href.value();

		if ((baseCurveID.length() > 1) &&
			(baseCurveID.at(0) == '#'))
		{
			baseCurveID = baseCurveID.substr(1);

			auto baseCurve = GetGeometry(baseCurveID);
			if (baseCurve) {

				auto type = baseCurve->GetType();
				if (type == GM::GeometryType::Curve) {
					auto orientableCurve = new GM::Curve(*((GM::Curve*)baseCurve));
					orientableCurve->SetID(gmlid);
					orientableCurve->orientation = bOrientation;
					return orientableCurve;
				}
				else if (type == GM::GeometryType::CompositeCurve) {
					auto orientableCurve = new GM::CompositeCurve(*((GM::CompositeCurve*)baseCurve));
					orientableCurve->SetID(gmlid);
					orientableCurve->orientation = bOrientation;
					return orientableCurve;
				}
				//else if (type == GM::GeometryType::OrientableCurve) {
				//	
				//}
			}
			return nullptr;
			/*auto object = new GM::OrientableCurve(baseCurveID);
			object->setParentIdSrsName(id, srsName);
			object->readIdSRSName(node);*/

			//return object;
			
		}
	}
	else {
		auto node_curve = node_baseCurve.first_child();
		std::string node_curve_name = node_curve.name();

		if (!node_curve_name.compare("S100:Curve")) {
			auto curve = ReadCurve(node_curve);
			if (curve) {
				//return (GM::OrientableCurve*)AddGeometry(curve);
				
			}
		}
		else if (!node_curve_name.compare("S100:CompositeCurve")) {
			auto compositeCurve = ReadCompositeCurve(node_curve);
			if (compositeCurve) {
				return (GM::OrientableCurve*)AddGeometry(compositeCurve);
			}
		}
		else if (!node_curve_name.compare("S100:OrientableCurve")) {
			auto orientableCurve = ReadOrientableCurve(node_curve);
			if (orientableCurve) {
				return (GM::OrientableCurve*)AddGeometry(orientableCurve);
			}
		}
	}

	return nullptr;
}

GM::CompositeCurve* S10XGML::ReadCompositeCurve(pugi::xml_node& node, std::string id, std::string srsName)
{
	auto object = new GM::CompositeCurve();
	object->setParentIdSrsName(id, srsName);
	object->readIdSRSName(node);

	auto child = node.child("gml:curveMember");

	while (child)
	{
		auto attr_href = child.attribute("xlink:href");
		if (attr_href) {
			std::string curveMemberID = attr_href.value();
			if (curveMemberID.length() > 1 &&
				curveMemberID.at(0) == '#')
			{
				curveMemberID = curveMemberID.substr(1);

				auto orientableCurve = GetOrientableCurve(curveMemberID);
				if (orientableCurve) {
					object->Add(orientableCurve);
				}
				else {
					CString str;
					str.Format(L"Failed to find curve (%s)\n", pugi::as_wide(curveMemberID).c_str());
					OutputDebugString(str);
				}
				//object->Add(curveMemberID);
			}
		}
		else {
			auto node_curve = child.first_child();
			std::string node_curve_name = node_curve.name();
			if (!node_curve_name.compare("S100:Curve")) {
				auto curve = ReadCurve(node_curve);
				if (curve) {
					auto addedCurve = AddGeometry(curve);
					if (addedCurve) {
						object->Add(curve);
					}
				}
			}
			else if (!node_curve_name.compare("S100:CompositeCurve")) {
				auto compositeCurve = ReadCompositeCurve(node_curve);
				if (compositeCurve) {
					auto addedCurve = AddGeometry(compositeCurve);
					if (addedCurve) {
						object->Add(compositeCurve);
					}
				}
			}
			else if (!node_curve_name.compare("S100:OrientableCurve")) {
				auto orientableCurve = ReadOrientableCurve(node_curve);
				if (orientableCurve) {
					auto addedCurve = AddGeometry(orientableCurve);
					if (addedCurve) {
						object->Add(orientableCurve);
					}
				}
			}
		}
		child = child.next_sibling("gml:curveMember");
	}

	return object;
}

GM::Surface* S10XGML::ReadSurface(pugi::xml_node& node)
{
	auto object = new GM::Surface();

	std::string gmlID = node.attribute("gml:id").value();
	std::string srsName = node.attribute("srsName").value();

	object->SetID(gmlID);

	// Exterior
	auto node_exterior = node.child("gml:patches").child("gml:PolygonPatch").child("gml:exterior");
	auto node_exterior_child = node_exterior.first_child();
	auto name_node_exterior_child = node_exterior_child.name();

	auto node_exterior_curveMember = node_exterior.child("gml:Ring").child("gml:curveMember");

	auto exterior_href = node_exterior_curveMember.attribute("xlink:href");
	if (exterior_href) {
		std::string exteriorCurveMemberID = exterior_href.value();
		if (exteriorCurveMemberID.length() > 1)
		{
			exteriorCurveMemberID = exteriorCurveMemberID.substr(1);
			object->SetExteriorRingID(exteriorCurveMemberID);
		}
	} 
	else if (!strcmp(name_node_exterior_child, "gml:LinearRing")) {
		auto curve = ReadLinearRing(node_exterior_child);
		if (curve) {
			object->SetExteriorRing(curve);
			delete curve;
			curve = nullptr;
		}
	}
	else {
		auto node_curve = node_exterior_curveMember.first_child();
		std::string node_curve_name = node_curve.name();
		if (!node_curve_name.compare("S100:Curve") || 
			!node_curve_name.compare("gml:Curve")) {
			auto curve = ReadCurve(node_curve);
			if (curve) {
				auto addedCurve = AddGeometry(curve);
				if (addedCurve) {
					object->SetExteriorRing(curve);
				}
			}
		}
		else if (!node_curve_name.compare("S100:CompositeCurve") ||
			!node_curve_name.compare("gml:CompositeCurve")) {
			auto compositeCurve = ReadCompositeCurve(node_curve);
			if (compositeCurve) {
				auto addedCurve = AddGeometry(compositeCurve);
				if (addedCurve) {
					object->SetExteriorRing(compositeCurve);
				}
			}
		}
		else if (!node_curve_name.compare("S100:OrientableCurve") ||
			!node_curve_name.compare("gml:OrientableCurve")) {
			auto orientableCurve = ReadOrientableCurve(node_curve);
			if (orientableCurve) {
				auto addedCurve = AddGeometry(orientableCurve);
				if (addedCurve) {
					object->SetExteriorRing(orientableCurve);
				}
			}
		}
	}

	// Interior
	auto node_interior = node.child("gml:patches").child("gml:PolygonPatch").child("gml:interior");
	while (node_interior)
	{
		auto node_interior_curveMember = node_interior.child("gml:Ring").child("gml:curveMember");
		auto interior_href = node_interior_curveMember.attribute("xlink:href");
		auto name_first_child = node_interior.first_child().name();

		if (interior_href) {
			std::string interiorCurveMemberID = interior_href.value();
			if (interiorCurveMemberID.length() > 1)
			{
				interiorCurveMemberID = interiorCurveMemberID.substr(1);
				object->AddInteriorRingID(interiorCurveMemberID);
			}
		}
		else if (!strcmp(name_first_child, "gml:LinearRing")) {
			auto curve = ReadLinearRing(node_interior.first_child());
			if (curve) {
				object->AddInteriorRing(curve);
				delete curve;
				curve = nullptr;
			}
		}
		else {
			auto node_curve = node_interior_curveMember.first_child();
			std::string node_curve_name = node_curve.name();
			if (!node_curve_name.compare("S100:Curve")) {
				auto curve = ReadCurve(node_curve);
				if (curve) {
					auto addedCurve = AddGeometry(curve);
					if (addedCurve) {
						object->AddInteriorRing(curve);
					}
				}
			}
			else if (!node_curve_name.compare("S100:CompositeCurve")) {
				auto compositeCurve = ReadCompositeCurve(node_curve);
				if (compositeCurve) {
					auto addedCurve = AddGeometry(compositeCurve);
					if (addedCurve) {
						object->AddInteriorRing(compositeCurve);
					}
				}
			}
			else if (!node_curve_name.compare("S100:OrientableCurve")) {
				auto orientableCurve = ReadOrientableCurve(node_curve);
				if (orientableCurve) {
					auto addedCurve = AddGeometry(orientableCurve);
					if (addedCurve) {
						object->AddInteriorRing(orientableCurve);
					}
				}
			}
		}

		node_interior = node_interior.next_sibling("gml:interior");
	}

	return object; 
}

bool S10XGML::ReadMember(pugi::xml_node& node)
{
	auto featureNode = node.first_child();
	std::string nodeName = featureNode.name();
	auto code = getCodeFromMember(nodeName);

	auto fc = GetFC();
	auto feature = fc->GetFeatureType(code);
	if (feature) {
		ReadFeature(featureNode, fc);
		return true;
	}

	return false;
}

bool S10XGML::ReadIMember(pugi::xml_node& node)
{
	auto informationNode = node.first_child();
	std::string nodeName = informationNode.name();
	auto code = getCodeFromMember(nodeName);

	auto fc = GetFC();
	auto information = fc->GetInformationType(code);
	if (information) {
		ReadInformation(informationNode, fc);
		return true;
	}

	return false;
}

bool S10XGML::ReadObjectAttribute(
	pugi::xml_node& node, GF::ObjectType* object, FeatureCatalogue* fc)
{
	auto sa = fc->GetSimpleAttribute(node.name());
	if (sa)
	{
		auto value = node.child_value();
		object->AddSimpleAttribute(sa->GetValueType(), sa->GetCode(), value);
	}
	else
	{
		auto ca = fc->GetComplexAttribute(node.name());
		if (ca)
		{
			auto addedCA = object->AddComplexAttribute(node.name());

			auto child = node.first_child();
			while (child)
			{
				AddSubAttribute(child, addedCA, fc);
				child = child.next_sibling();
			}
		}
	}

	return true;
}

bool S10XGML::AddSubAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute, FeatureCatalogue* fc)
{
	auto sa = fc->GetSimpleAttribute(node.name());
	if (sa)
	{
		auto value = node.child_value();
		complexAttribute->AddSubSimpleAttribute(sa->GetValueType(), sa->GetCode(), value);
	}
	else
	{
		auto ca = fc->GetComplexAttribute(node.name());
		if (ca)
		{
			auto addedCA = complexAttribute->AddComplexAttribute(node.name());

			auto child = node.first_child();
			while (child)
			{
				AddSubAttribute(child, addedCA, fc);
				child = child.next_sibling();
			}
		}
	}

	return true;
}

bool S10XGML::ReadFeatureGeometry(pugi::xml_node& node, GF::FeatureType* feature)
{
	std::string geometryID = node.first_child().attribute("xlink:href").value();
	if (geometryID.length() > 1)
	{
		feature->SetGeometryID(geometryID.substr(1));
	}
	else
	{
		auto geomNode = node.first_child();
		std::string nodeName = geomNode.name();

		if ((nodeName.find("pointProperty") != std::string::npos) || 
			(nodeName.find("S100:pointProperty") != std::string::npos))
		{
			auto nodePoint = geomNode.first_child();
			auto point = ReadPoint(nodePoint);
			if (point) 
			{
				auto addedPoint = AddGeometry(point);
				if (addedPoint) {
					feature->SetGeometryID(addedPoint->GetID());
				}
			}
		}
		else if ((nodeName.find("curveProperty") != std::string::npos) ||
			(nodeName.find("S100:curveProperty") != std::string::npos)) 
		{
			auto nodeCurve = geomNode.first_child();
			auto curve = ReadCurve(nodeCurve);
			if (curve) {
				auto addedCurve = AddGeometry(curve);
				if (addedCurve) {
					feature->SetGeometryID(addedCurve->GetID());
				}
			}
		}
		else if ((nodeName.find("surfaceProperty") != std::string::npos) || 
			(nodeName.find("S100:surfaceProperty") != std::string::npos))
		{
			auto nodeSurface = geomNode.first_child();
			auto surface = ReadSurface(nodeSurface);
			if (surface)
			{
				auto addedSurface = AddGeometry(surface);
				if (addedSurface) {
					feature->SetGeometryID(addedSurface->GetID());
				}
			}
		}
	}

	return true;
}

bool S10XGML::ReadFeatureRole(pugi::xml_node& node, GF::FeatureType* feature, FeatureCatalogue* fc)
{
	//auto role = fc->GetRole(node.name());
	//if (role)
	{
		std::string associatedID = node.attribute("xlink:href").value();
		if (associatedID.length() > 1)
		{
			associatedID = associatedID.substr(1);
			std::string associationCode = node.attribute("xlink:title").value();
			auto fa = fc->GetFeatureAssociation(associationCode);
			if (fa)
			{
				feature->AddFeatureAssociation(associationCode, node.name(), associatedID);
				return true;
			}
			else
			{
				auto ia = fc->GetInformationAssociation(associationCode);
				if (ia)
				{
					feature->AddInformationAssociation(associationCode, node.name(), associatedID);
					return true;
				}
			}
		}
	}

	return false;
}

bool S10XGML::ReadInformationRole(
	pugi::xml_node& node, GF::InformationType* information, FeatureCatalogue* fc)
{
	auto role = fc->GetRole(node.name());
	if (role)
	{
		std::string associatedID = node.attribute("xlink:href").value();
		if (associatedID.length() > 1)
		{
			associatedID = associatedID.substr(1);
			std::string associationCode = node.attribute("xlink:title").value();
			auto ia = fc->GetInformationAssociation(associationCode);
			if (ia)
			{
				information->AddInformationAssociation(associationCode, node.name(), associatedID);
				return true;
			}
		}
	}

	return false;
}

GM::Point* S10XGML::GetPoint(int x, int y)
{
	for (auto i = geometries.begin(); i != geometries.end(); i++)
	{
		if ((*i)->GetType() == GM::GeometryType::Point)
		{
			auto pt = (GM::Point*)(*i);
			if (pt->position.GetXInteger() == x && pt->position.GetYInteger() == y)
			{
				return pt;
			}
		}
	}

	return nullptr;
}

GM::Object* S10XGML::AddGeometry(GM::Object* geometry)
{
	if (geometry) {
		auto geometryID = geometry->GetID();

		auto find = GetGeometry(geometryID);
		if (find) {
			//GM::Object* newGeometry = nullptr;
			//if (typeid(GM::Point) == typeid(*geometry)) {
			//	newGeometry = new GM::Point((GM::Point&)*geometry);
			//}
			//else if (typeid(GM::MultiPoint) == typeid(*geometry)) {
			//	newGeometry = new GM::MultiPoint((GM::MultiPoint&)*geometry);
			//}
			//else if (typeid(GM::OrientableCurve) == typeid(*geometry)) {
			//	newGeometry = new GM::OrientableCurve((GM::OrientableCurve&)*geometry);
			//}
			//else if (typeid(GM::Curve) == typeid(*geometry)) {
			//	newGeometry = new GM::Curve((GM::Curve&)*geometry);
			//}
			//else if (typeid(GM::CompositeCurve) == typeid(*geometry)) {
			//	newGeometry = new GM::CompositeCurve((GM::CompositeCurve&)*geometry);
			//}
			//else if (typeid(GM::Surface) == typeid(*geometry)) {
			//	newGeometry = new GM::Surface((GM::Surface&)*geometry);
			//}
			//newGeometry->SetID(LatLonUtility::generate_uuid());
			//geometries.push_back(newGeometry);
			//return newGeometry;
			geometry->SetID(LatLonUtility::generate_uuid());
			geometries.push_back(geometry);
			return geometry;
		}
		else {
			geometries.push_back(geometry);
			return geometry;
		}
	}

	return nullptr;
}

bool S10XGML::WriteInputXML_FeatureTypes(pugi::xml_node& node)
{
	for (auto i = features.begin(); i != features.end(); i++) {
		auto feature = *i;

		auto node_feature = node.append_child(feature->GetCode().c_str());
		node_feature.append_attribute("id").set_value(feature->GetID().c_str());

		auto geom_id = feature->GetGeometryID();
		auto geometry = GetGeometry(geom_id);
		if (geometry) {
			node_feature.append_attribute("primitive").set_value(geometry->GeometricPrimitiveAsString().c_str());
			node_feature.append_child(geometry->GeometricPrimitiveAsString().c_str()).append_attribute("ref").set_value(geometry->GetID().c_str());
		}

		for (auto j = feature->attributes.begin(); j != feature->attributes.end(); j++) {
			auto attr = (*j);
			if (attr->IsSimple()) {
				WriteInputXML_FeatureType_SimpleAttribute(node_feature, (GF::SimpleAttributeType*)attr);
			}
			else {
				auto complex_node = node_feature.append_child(attr->GetCode().c_str());
				WriteInputXML_FeatureType_ComplexAttribute(complex_node, (GF::ComplexAttributeType*)attr);
			}
		}

		auto cntInformationAssociation = feature->GetInformationRelationCount();
		for (int i = 0; i < cntInformationAssociation; i++) {
			auto informationAssociation = feature->getInformationAssociation(i);
			auto informationAssociationCode = informationAssociation.GetCode();
			if (!informationAssociationCode.empty()) {
				auto node_association = node_feature.append_child(informationAssociationCode.c_str()); 
				node_association.append_attribute("role").set_value(informationAssociation.GetRole().c_str());
				node_association.append_attribute("informationRef").set_value(informationAssociation.GetInformationID().c_str());
			}
		}
	}

	return true;
}

bool S10XGML::WriteInputXML_InformationTypes(pugi::xml_node& node)
{
	for (auto i = informations.begin(); i != informations.end(); i++) {
		auto information = *i;

		auto node_information = node.append_child(information->GetCode().c_str());
		node_information.append_attribute("id").set_value(information->GetID().c_str());

		for (auto j = information->attributes.begin(); j != information->attributes.end(); j++) {
			auto attr = (*j);
			if (attr->IsSimple()) {
				WriteInputXML_FeatureType_SimpleAttribute(node_information, (GF::SimpleAttributeType*)attr);
			}
			else {
				auto complex_node = node_information.append_child(attr->GetCode().c_str());
				WriteInputXML_FeatureType_ComplexAttribute(complex_node, (GF::ComplexAttributeType*)attr);
			}
		}
	}

	return true;
}

bool S10XGML::WriteInputXML_FeatureType_SimpleAttribute(pugi::xml_node& node, GF::SimpleAttributeType* simpleAttribute)
{
	auto fc = GetFC();
	if (!fc) return false;

	auto code = simpleAttribute->GetCode();

	auto type = fc->getSimpleAttributeType(code);
	if (type == FCD::S100_CD_AttributeValueType::none) {
		return false;
	}
	else if (type == FCD::S100_CD_AttributeValueType::enumeration) {
		auto sa = fc->GetSimpleAttribute(code);
		if (sa) {
			auto lv = sa->GetListedValue(simpleAttribute->GetValue());
			if (lv) {
				auto value = lv->GetCode();
				node.append_child(code.c_str()).append_child(pugi::node_pcdata).set_value(std::to_string(value).c_str());
			}
		}
		else {
			return false;
		}
	}
	else {
		node.append_child(code.c_str()).append_child(pugi::node_pcdata).set_value(simpleAttribute->GetValue().c_str());
	}

	return true;
}

bool S10XGML::WriteInputXML_FeatureType_ComplexAttribute(pugi::xml_node& node, GF::ComplexAttributeType* complexAttribute)
{
	for (int i = 0; i < complexAttribute->GetSubAttributeCount(); i++) {
		auto subAttr = complexAttribute->GetSubAttribute(i);
		if (subAttr->IsSimple()) {
			WriteInputXML_FeatureType_SimpleAttribute(node, (GF::SimpleAttributeType*)subAttr);
		}
		else {
			auto complex_node = node.append_child(subAttr->GetCode().c_str());
			WriteInputXML_FeatureType_ComplexAttribute(complex_node, (GF::ComplexAttributeType*)subAttr);
		}
	}

	return true;
}

GF::FeatureType* S10XGML::GetFeatureType(std::string id)
{
	for (auto i = features.begin(); i != features.end(); i++) {
		auto feature = (*i);
		if (!feature->GetID().compare(id)) {
			return feature;
		}
	}

	return nullptr;
}

GF::FeatureType* S10XGML::GetFeatureTypeByIndex(int index)
{
	if (index < 0 || index >= features.size()) {
		return nullptr;
	}

	return features.at(index);
}

GF::InformationType* S10XGML::GetInformationType(std::string id)
{
	for (auto i = informations.begin(); i != informations.end(); i++) {
		auto information = (*i);
		if (!information->GetID().compare(id)) {
			return information;
		}
	}

	return nullptr;
}

GF::InformationType* S10XGML::GetInformationTypeByIndex(int index)
{
	if (index < 0 || index >= informations.size()) {
		return nullptr;
	}

	return informations.at(index);
}

std::string S10XGML::GetFeatureAssociationCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetFeatureRelationCount()) {
		return std::string("");
	}
	
	auto fa = featureType->getFeatureAssociation(index);
	if (!fa.GetFeatureID().empty()) {
		return fa.GetCode();
	}

	return std::string("");
}

std::string S10XGML::GetFeatureAssociationRoleCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetFeatureRelationCount()) {
		return std::string("");
	}
	
	auto fa = featureType->getFeatureAssociation(index);
	if (!fa.GetFeatureID().empty()) {
		return fa.GetRole();
	}

	return std::string("");
}

std::string S10XGML::GetInformationAssociationCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetInformationRelationCount()) {
		return std::string("");
	}

	auto fa = featureType->getInformationAssociation(index);
	if (!fa.GetInformationID().empty()) {
		return fa.GetCode();
	}

	return std::string("");
}

std::string S10XGML::GetInformationAssociationRoleCode(GF::FeatureType* featureType, int index)
{
	if (index < 0 || index >= featureType->GetInformationRelationCount()) {
		return std::string("");
	}

	auto fa = featureType->getInformationAssociation(index);
	if (!fa.GetInformationID().empty()) {
		return fa.GetRole();
	}

	return std::string("");
}

std::string S10XGML::GetInformationAssociationCode(GF::InformationType* informationType, int index)
{
	if (index < 0 || index >= informationType->GetInformationRelationCount()) {
		return std::string("");
	}

	auto fa = informationType->getInformationAssociation(index);
	if (!fa.GetInformationID().empty()) {
		return fa.GetCode();
	}

	return std::string("");
}

std::string S10XGML::GetInformationAssociationRoleCode(GF::InformationType* informationType, int index)
{
	if (index < 0 || index >= informationType->GetInformationRelationCount()) {
		return std::string("");
	}

	auto fa = informationType->getInformationAssociation(index);
	if (!fa.GetInformationID().empty()) {
		return fa.GetRole();
	}

	return std::string("");
}

//std::string S10XGML::GetObjectAttributeCode(int type, std::string id, int index)
//{
//	if (type == 1)
//	{
//		return GetFeatureAttributeCode(id, index);
//	}
//	else if (type == 2)
//	{
//		return GetInformationAttributeCode(id, index);
//	}
//
//	return "";
//}

//int S10XGML::GetFeatureAttributeCount(std::string id)
//{
//	
//}

//std::string S10XGML::GetFeatureAttributeCode(std::string, int index)
//{
//	
//}

//int S10XGML::GetInformationAttributeCount(std::string id)
//{
//	
//}

//std::string S10XGML::GetInformationAttributeCode(std::string, int index)
//{
//	
//}

std::string S10XGML::DeleteXMLNamespace(std::string value)
{
	size_t pos = value.find(':');
	if (pos != std::string::npos) {
		value.erase(0, pos + 1);
	}

	return value;
}

std::string S10XGML::getCodeFromMember(std::string nodeName)
{
	auto found = nodeName.find(':');

	if (found != std::string::npos) {
		std::string subStr = nodeName.substr(found + 1);  
		return subStr;
	}

	return nodeName;
}

void S10XGML::CalcMBR()
{
	auto layer = GetLayer();
	if (layer) {
		auto mbr = layer->GetMBRPointer();
		if (mbr) {
			mbr->InitMBR();

			for (auto i = features.begin(); i != features.end(); i++) {
				auto feature = (*i);
				auto geometry = feature->GetGeometry();
				if (geometry) {
					auto geomMBR = geometry->GetMBR();
					mbr->ReMBR(geomMBR);
				}
			}
		}
	}
}

void S10XGML::SetGeometry()
{
	for (auto i = features.begin(); i != features.end(); i++) {
		auto feature = (*i);
		auto geometryID = feature->GetGeometryID();
		auto geometry = GetGeometry(geometryID);
		if (geometry) {
			if (geometry->GetType() == GM::GeometryType::Point) {
				auto sPoint = PointToSPoint((GM::Point*)geometry);
				feature->SetGeometry(sPoint);
				sPoint->CreateD2Geometry(D2->Factory());
			}
			else if (geometry->GetType() == GM::GeometryType::MultiPoint) {
				auto sMultiPoint = MultiPointToSMultiPoint((GM::MultiPoint*)geometry);
				feature->SetGeometry(sMultiPoint);
				sMultiPoint->CreateD2Geometry(D2->Factory());
			}
			else if (geometry->GetType() == GM::GeometryType::OrientableCurve) {
				auto sCurve = OrientableCurveToSCurve((GM::OrientableCurve*)geometry);
				feature->SetGeometry(sCurve);
				sCurve->CreateD2Geometry(D2->Factory());
			}
			else if (geometry->GetType() == GM::GeometryType::Curve) {
				auto sCurve = CurveToSCurve((GM::Curve*)geometry);
				feature->SetGeometry(sCurve);
				sCurve->CreateD2Geometry(D2->Factory());
			}
			else if (geometry->GetType() == GM::GeometryType::CompositeCurve) {
				auto sCompositeCurve = CompositeCurveToSCompositeCurve((GM::CompositeCurve*)geometry);
				feature->SetGeometry(sCompositeCurve);
				sCompositeCurve->CreateD2Geometry(D2->Factory());
			}
			else if (geometry->GetType() == GM::GeometryType::Surface) {
				auto sSurface = SurfaceToSSurface((GM::Surface*)geometry);
				feature->SetGeometry(sSurface);
				sSurface->CreateD2Geometry(D2->Factory());

				auto surface = (GM::Surface*)geometry;
				auto exteriorRing = surface->getExteriorRing();
				std::vector<GM::Ring*> rings;
				rings.push_back(exteriorRing);

				for (int j = 0; j < surface->getInteriorRingCount(); j++) {
					auto interiorRing = surface->getInteriorRing(j);
					rings.push_back(interiorRing);
				}

				for (int j = 0; j < rings.size(); j++) {
					auto ring = rings[j];
					if (ring->GetType() == GM::GeometryType::Curve) {
						auto sCurve = CurveToSCurve((GM::Curve*)ring);
						sCurve->CreateD2Geometry(D2->Factory());
						sSurface->AddCurve(sCurve);
					}
					else if (ring->GetType() == GM::GeometryType::CompositeCurve) {
						auto sCompositeCurve = CompositeCurveToSCompositeCurve((GM::CompositeCurve*)ring);
						sCompositeCurve->CreateD2Geometry(D2->Factory());
						sSurface->AddCurve(sCompositeCurve);
					}
				}
			}
		}
	}
}

SPoint* S10XGML::PointToSPoint(GM::Point* point)
{
	auto x = point->position.GetX();
	auto y = point->position.GetY();
	projection(x, y);

	SPoint* result = new SPoint(x, y);
	return result;
}

SMultiPoint* S10XGML::MultiPointToSMultiPoint(GM::MultiPoint* multiPoint)
{
	SMultiPoint* result = new SMultiPoint();

	for (auto i = multiPoint->position.begin(); i != multiPoint->position.end(); i++) {
		auto x = i->GetX();
		auto y = i->GetY();
		auto z = i->GetZ();
		projection(x, y);
		result->Add(x, y, z);
	}

	result->SetMBR();

	return result;
}

SAbstractCurve* S10XGML::OrientableCurveToSCurve(GM::OrientableCurve* orientableCurve)
{
	//auto baseCurveID = orientableCurve->GetBaseCurveID();
	auto baseCurveID = orientableCurve->GetID();

	if (baseCurveID.find('#') != std::string::npos) {
		baseCurveID = baseCurveID.substr(1);
	}

	auto baseCurve = GetGeometry(baseCurveID);
	
	if (baseCurve->GetType() == GM::GeometryType::OrientableCurve) {
		return OrientableCurveToSCurve((GM::OrientableCurve*)baseCurve);
	}
	else if (baseCurve->GetType() == GM::GeometryType::Curve) {
		return CurveToSCurve((GM::Curve*)baseCurve);
	}
	else if (baseCurve->GetType() == GM::GeometryType::CompositeCurve) {
		return CompositeCurveToSCompositeCurve((GM::CompositeCurve*)baseCurve);
	}

	return nullptr;
}

SCurve* S10XGML::CurveToSCurve(GM::Curve* curve)
{
	SCurve* result = new SCurve();
	auto pointCount = curve->getPointCount();
	result->Init(pointCount);

	for (auto i = 0; i < pointCount; i++) {
		auto x = curve->getX(i);
		auto y = curve->getY(i);
		projection(x, y);

		result->Set(i, x, y);
	}

	result->SetMBR();

	return result;
}

SCompositeCurve* S10XGML::CompositeCurveToSCompositeCurve(GM::CompositeCurve* compositeCurve)
{
	SCompositeCurve* result = new SCompositeCurve();

	for (auto i = compositeCurve->component.begin(); i != compositeCurve->component.end(); i++) {
		auto component = (*i);
		if (component->GetType() == GM::GeometryType::OrientableCurve) {
			auto curveSegment = OrientableCurveToSCurve((GM::OrientableCurve*)component);
			result->AddCurve(curveSegment);
		}
		else if (component->GetType() == GM::GeometryType::Curve) {
			auto curveSegment = CurveToSCurve((GM::Curve*)component);
			result->AddCurve(curveSegment);
		}
		else if (component->GetType() == GM::GeometryType::CompositeCurve) {
			auto curveSegment = CompositeCurveToSCompositeCurve((GM::CompositeCurve*)component);
			result->AddCurve(curveSegment);
		}
	}

	result->SetMBR();

	return result;
}

SSurface* S10XGML::SurfaceToSSurface(GM::Surface* surface)
{
	std::vector<POINT> points;
	std::vector<int> parts;

	auto result = new SSurface();

	auto exteriorRing = CompositeCurveToSCompositeCurve(surface->getExteriorRing());

	auto pointCountExteriorRing = exteriorRing->GetPointCount();
	for (auto i = 0; i < pointCountExteriorRing; i++) {
		auto x = exteriorRing->GetX(i);
		auto y = exteriorRing->GetY(i);
		
		inverseProjection(x, y);

		POINT pt = { (LONG)(x * 10000000), (LONG)(y * 10000000) };

		points.push_back(pt);
	}

	parts.push_back((int)points.size());
	delete exteriorRing;
	exteriorRing = nullptr;

	auto numInteriorRing = surface->getInteriorRingCount();
	for (int i = 0; i < numInteriorRing; i++) {
		auto currentInteriorRing = surface->getInteriorRing(i);

		if (currentInteriorRing) {

			auto interiorRing = CompositeCurveToSCompositeCurve(currentInteriorRing);
			auto pointCountInteriorRing = interiorRing->GetPointCount();
			
			

			for (auto i = 0; i < pointCountInteriorRing; i++) {
				auto x = interiorRing->GetX(i);
				auto y = interiorRing->GetY(i);

				inverseProjection(x, y);

				POINT pt = { (LONG)(x * 10000000), (LONG)(y * 10000000) };

				points.push_back(pt);
			}
			
			parts.push_back((int)points.size());
			delete interiorRing;
			interiorRing = nullptr;
		}
	}

	//auto boundary = surface->GetPolygon().boundary;
	//auto interior = boundary.interior;

	//auto i_begin = interior.begin();
	//auto i_end = interior.end();

	//for (auto i = i_begin;
	//	i != i_end;
	//	i++) {
	//	auto interiorRing = CompositeCurveToSCompositeCurve(*i);
	//	auto pointCountInteriorRing = interiorRing->GetPointCount();
	//	for (auto i = 0; i < pointCountInteriorRing; i++) {
	//		auto x = interiorRing->GetX(i);
	//		auto y = interiorRing->GetY(i);

	//		inverseProjection(x, y);

	//		POINT pt = { x * 10000000, y * 10000000 };

	//		points.push_back(pt);
	//	}

	//	parts.push_back(pointCountInteriorRing);
	//}

	result->Set(points, parts);

	return result;
}