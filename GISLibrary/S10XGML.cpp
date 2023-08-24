#include "stdafx.h"
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

S10XGML::S10XGML()
{
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
		delete (*i);
	}
}

bool S10XGML::Open(CString _filepath)
{
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
			
		}
		else if (strcmp(child.name(), "member") == 0)
		{
			ReadMember(child);
		}

		child = child.next_sibling();
	}

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

	auto infomationTypes = root.append_child("InformationTypes");
	auto featureTypes = root.append_child("Features");

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
	while (child)
	{
		std::string childName = child.name();

		auto attribute = fc->GetAttribute(childName);
		if (attribute)
		{
			ReadObjectAttribute(child, feature, fc);
		}
		else if (childName.compare("geometry") == 0)
		{
			ReadFeatureGeometry(child, feature);
		}
		else
		{
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

GM::Point* S10XGML::ReadPoint(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();
	std::string srsName = node.attribute("srsName").value();

	auto strPos = node.child_value("gml:pos");
		
	auto strPosList = LatLonUtility::Split(strPos, " ");

	if (strPosList.size() != 2)
	{
		return false;
	}

	int latIndex = 0;
	int lonIndex = 1;

	if (srsName.length() == 0)
	{
		latIndex = 1;
		lonIndex = 0;
	}

	double lat = std::stod(strPosList.at(latIndex));
	double lon = std::stod(strPosList.at(lonIndex));

	auto object = new GM::Point();
	object->SetID(gmlID);
	object->position.SetX(lon);
	object->position.SetY(lat);

	return object;
}

GM::MultiPoint* S10XGML::ReadMultiPoint(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();

	auto nodePoint = node.child("gml:pointMembers").child("gml:Point");

	auto object = new GM::MultiPoint();
	object->SetID(gmlID);

	while (nodePoint)
	{
		auto strPos = nodePoint.child_value("gml:pos");

		auto strPosList = LatLonUtility::Split(strPos, " ");

		if (strPosList.size() == 3)
		{
			double lat = std::stod(strPosList.at(0));
			double lon = std::stod(strPosList.at(1));
			double depth = std::stod(strPosList.at(2));
			
			object->Add(lon, lat, depth);
		}

		nodePoint = nodePoint.next_sibling();
	}

	return object;
}

GM::Curve* S10XGML::ReadCurve(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();

	auto strPos = node.child("gml:Segment").child("gml:LineStringSegment").child_value("gml:posList");

	auto strPosList = LatLonUtility::Split(strPos, " ");
	int posCnt = strPosList.size();

	if (posCnt < 4 && posCnt % 2 != 0)
	{
		return false;
	}

	auto object = new GM::Curve();
	object->SetID(gmlID);

	for (int i = 0; i < posCnt; i += 2)
	{
		double lat = std::stod(strPosList.at(i));
		double lon = std::stod(strPosList.at(i + 1));
		object->Add(lon, lat);
	}

	return object;
}

GM::OrientableCurve* S10XGML::ReadOrientableCurve(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();
	auto strOrientation = node.attribute("orientation").value();
	if (strcmp(strOrientation, "-") != 0)
	{
		return false;
	}

	auto node_baseCurve = node.child("gml:baseCurve");
	auto attr_baseCurve_href = node_baseCurve.attribute("xlink:href");

	if (attr_baseCurve_href) {
		std::string baseCurveID = attr_baseCurve_href.value();

		if ((baseCurveID.length() > 1) &&
			(baseCurveID.at(0) == '#'))
		{
			baseCurveID = baseCurveID.substr(1);

			auto object = new GM::OrientableCurve(baseCurveID);
			object->SetID(gmlID);

			return object;
		}
	}
	else {
		auto node_curve = node_baseCurve.first_child();
		std::string node_curve_name = node_curve.name();

		if (!node_curve_name.compare("S100:Curve")) {
			auto curve = ReadCurve(node_curve);
			if (curve) {
				AddGeometry(curve);
				return curve;
			}
		}
		else if (!node_curve_name.compare("S100:CompositeCurve")) {
			auto compositeCurve = ReadCompositeCurve(node_curve);
			if (compositeCurve) {
				AddGeometry(compositeCurve);
				return compositeCurve;
			}
		}
		else if (!node_curve_name.compare("S100:OrientableCurve")) {
			auto orientableCurve = ReadOrientableCurve(node_curve);
			if (orientableCurve) {
				AddGeometry(orientableCurve);
				return orientableCurve;
			}
		}
	}

	return nullptr;
}

GM::CompositeCurve* S10XGML::ReadCompositeCurve(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();
	auto child = node.child("gml:curveMember");

	auto object = new GM::CompositeCurve();
	object->SetID(gmlID);

	while (child)
	{
		auto attr_href = child.attribute("xlink:href");
		if (attr_href) {
			std::string curveMemberID = attr_href.value();
			if (curveMemberID.length() > 1 &&
				curveMemberID.at(0) == '#')
			{
				object->Add(curveMemberID);
			}
		}
		else {
			auto node_curve = child.first_child();
			std::string node_curve_name = node_curve.name();
			if (!node_curve_name.compare("S100:Curve")) {
				auto curve = ReadCurve(node_curve);
				if (curve) {
					object->Add(*curve);
					AddGeometry(curve);
				}
			}
			else if (!node_curve_name.compare("S100:CompositeCurve")) {
				auto compositeCurve = ReadCompositeCurve(node_curve);
				if (compositeCurve) {
					object->Add(*compositeCurve);
					AddGeometry(compositeCurve);
				}
			}
			else if (!node_curve_name.compare("S100:OrientableCurve")) {
				auto orientableCurve = ReadOrientableCurve(node_curve);
				if (orientableCurve) {
					object->Add(*orientableCurve);
					AddGeometry(orientableCurve);
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
	else {
		auto node_curve = node_exterior_curveMember.first_child();
		std::string node_curve_name = node_curve.name();
		if (!node_curve_name.compare("S100:Curve")) {
			auto curve = ReadCurve(node_curve);
			if (curve) {
				object->SetExteriorRing(*curve);
				AddGeometry(curve);
			}
		}
		else if (!node_curve_name.compare("S100:CompositeCurve")) {
			auto compositeCurve = ReadCompositeCurve(node_curve);
			if (compositeCurve) {
				object->SetExteriorRing(*compositeCurve);
				AddGeometry(compositeCurve);
			}
		}
		else if (!node_curve_name.compare("S100:OrientableCurve")) {
			auto orientableCurve = ReadOrientableCurve(node_curve);
			if (orientableCurve) {
				object->SetExteriorRing(*orientableCurve);
				AddGeometry(orientableCurve);
			}
		}
	}

	// Interior
	auto node_interior = node.child("gml:patches").child("gml:PolygonPatch").child("gml:interior");
	while (node_interior)
	{
		auto node_interior_curveMember = node_interior.child("gml:Ring").child("gml:curveMember");
		auto interior_href = node_interior_curveMember.attribute("xlink:href");

		if (interior_href) {
			std::string interiorCurveMemberID = interior_href.value();
			if (interiorCurveMemberID.length() > 1)
			{
				interiorCurveMemberID = interiorCurveMemberID.substr(1);
				object->AddInteriorRingID(interiorCurveMemberID);
			}
		}
		else {
			auto node_curve = node_interior_curveMember.first_child();
			std::string node_curve_name = node_curve.name();
			if (!node_curve_name.compare("S100:Curve")) {
				auto curve = ReadCurve(node_curve);
				if (curve) {
					object->AddInteriorRing(*curve);
					AddGeometry(curve);
				}
			}
			else if (!node_curve_name.compare("S100:CompositeCurve")) {
				auto compositeCurve = ReadCompositeCurve(node_curve);
				if (compositeCurve) {
					object->AddInteriorRing(*compositeCurve);
					AddGeometry(compositeCurve);
				}
			}
			else if (!node_curve_name.compare("S100:OrientableCurve")) {
				auto orientableCurve = ReadOrientableCurve(node_curve);
				if (orientableCurve) {
					object->AddInteriorRing(*orientableCurve);
					AddGeometry(orientableCurve);
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

		if (nodeName.find("pointProperty") != std::string::npos)
		{
			auto nodePoint = geomNode.first_child();
			auto point = ReadPoint(nodePoint);
			if (point) 
			{
				AddGeometry(point);
				feature->SetGeometryID(point->GetID());
			}
		}
		else if (nodeName.find("surfaceProperty") != std::string::npos)
		{
			auto nodeSurface = geomNode.first_child();
			auto surface = ReadSurface(nodeSurface);
			if (surface)
			{
				AddGeometry(surface);
				feature->SetGeometryID(surface->GetID());
			}
		}
	}

	return true;
}

bool S10XGML::ReadFeatureRole(pugi::xml_node& node, GF::FeatureType* feature, FeatureCatalogue* fc)
{
	auto role = fc->GetRole(node.name());
	if (role)
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

void S10XGML::AddGeometry(GM::Object* geometry)
{
	if (geometry) {
		geometries.push_back(geometry);
	}
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
	}

	return true;
}

bool S10XGML::WriteInputXML_FeatureType_SimpleAttribute(pugi::xml_node& node, GF::SimpleAttributeType* simpleAttribute)
{
	node.append_child(simpleAttribute->GetCode().c_str()).append_child(pugi::node_pcdata).set_value(simpleAttribute->GetValue().c_str());

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
	auto found = nodeName.find(':');  // ':' 문자 검색

	if (found != std::string::npos) {
		std::string subStr = nodeName.substr(found + 1);  // ':' 이후의 부분 문자열 추출
		return subStr;
	}

	return nodeName;
}