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

	auto productNumber = GetProductNumber();
	std::string rootName = "S" + std::to_string(productNumber) + ":Dataset";

	pugi::xml_node root = doc.child(rootName.c_str());

	auto child = root.first_child();
	while (child)
	{
		if (strcmp(child.name(), "gml:boundedBy") == 0)
		{
			envelop.Read(child);
		}
		else if (strcmp(child.name(), "S100:DatasetIdentificationInformation") == 0)
		{
			datasetIdentificationInformation.Read(child);
		}
		else if (strcmp(child.name(), "members") == 0)
		{
			ReadMembers(child);
		}
		else if (strcmp(child.name(), "S100:Point") == 0)
		{
			ReadPoint(child);
		}
		else if (strcmp(child.name(), "S100:MultiPoint") == 0)
		{
			ReadMultiPoint(child);
		}
		else if (strcmp(child.name(), "S100:Curve") == 0)
		{
			ReadCurve(child);
		}	
		else if (strcmp(child.name(), "S100:OrientableCurve") == 0)
		{
			ReadOrientableCurve(child);
		}
		else if (strcmp(child.name(), "S100:CompositeCurve") == 0)
		{
			ReadCompositeCurve(child);
		}
		else if (strcmp(child.name(), "S100:Surface") == 0)
		{
			ReadSurface(child);
		}

		child = child.next_sibling();
	}

	return true;
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
			ReadFeature(child, fc, feature);
		}
		else
		{
			auto info = fc->GetInformationType(code);
			if (info)
			{
				ReadInformation(child, fc, info);
			}
		}

		child = child.next_sibling();
	}

	return true;
}

bool S10XGML::ReadFeature(pugi::xml_node& node, FeatureCatalogue* fc, FeatureType* featureType)
{
	auto feature = new GF::FeatureType();

	feature->id = node.attribute("gml:id").value();
	feature->code = node.name();

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
	return true;
}

bool S10XGML::ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc, InformationType* informationType)
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
	return true;
}

bool S10XGML::ReadPoint(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();

	auto strPos = node.child_value("gml:pos");
		
	auto strPosList = LatLonUtility::Split(strPos, " ");

	if (strPosList.size() != 2)
	{
		return false;
	}

	double lat = std::stod(strPosList.at(0));
	double lon = std::stod(strPosList.at(1));

	auto object = new GM::Point();
	object->SetID(gmlID);
	object->position.SetX(lon);
	object->position.SetY(lat);

	geometries.push_back(object);
		
	return true;
}

bool S10XGML::ReadMultiPoint(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();

	auto strPos = node.child("gml:pointMembers").child("gml:Point").child_value("gml:pos");

	auto strPosList = LatLonUtility::Split(strPos, " ");

	if (strPosList.size() != 3)
	{
		return false;
	}

	double lat = std::stod(strPosList.at(0));
	double lon = std::stod(strPosList.at(1));
	double depth = std::stod(strPosList.at(2));

	auto object = new GM::MultiPoint();
	object->SetID(gmlID);
	object->Set(lon, lat, depth);

	geometries.push_back(object);

	return true;
}

bool S10XGML::ReadCurve(pugi::xml_node& node)
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
	geometries.push_back(object);

	for (int i = 0; i < posCnt; i += 2)
	{
		double lat = std::stod(strPosList.at(i));
		double lon = std::stod(strPosList.at(i + 1));
		object->Add(lon, lat);
	}

	return true;
}

bool S10XGML::ReadOrientableCurve(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();
	auto strOrientation = node.attribute("orientation").value();
	if (strcmp(strOrientation, "-") != 0)
	{
		return false;
	}

	std::string baseCurveID = node.child("gml:baseCurve").attribute("xlink:href").value();

	if ((baseCurveID.length() > 1) && 
		(baseCurveID.at(0) == '#'))
	{
		baseCurveID = baseCurveID.substr(1);

		auto object = new GM::OrientableCurve(baseCurveID);
		object->SetID(gmlID);
		geometries.push_back(object);

		return true;
	}

	return false;
}

bool S10XGML::ReadCompositeCurve(pugi::xml_node& node)
{
	std::string gmlID = node.attribute("gml:id").value();
	auto child = node.child("gml:curveMember");

	auto object = new GM::CompositeCurve();
	object->SetID(gmlID);

	while (child)
	{
		std::string curveMemberID = child.attribute("xlink:href").value();
		if (curveMemberID.length() <= 1 &&
			curveMemberID.at(0) != '#')
		{
			delete object;
			return false;
		}

		object->Add(curveMemberID);
		child = child.next_sibling("gml:curveMember");
	}

	geometries.push_back(object);

	return true;
}

bool S10XGML::ReadSurface(pugi::xml_node& node)
{
	std::string exteriorCurveMemberID = node.child("gml:patches").child("gml:PolygonPatch")
		.child("gml:exterior").child("gml:Ring").child("gml:curveMember").attribute("xlink:href").value();

	if (exteriorCurveMemberID.length() <= 1)
	{
		return false;
	}
	else
	{
		exteriorCurveMemberID = exteriorCurveMemberID.substr(1);
	}

	auto object = new GM::Surface();

	std::string gmlID = node.attribute("gml:id").value();
	object->SetID(gmlID);
	geometries.push_back(object);

	object->SetExteriorRingID(exteriorCurveMemberID);

	auto nodeInterior = node.child("gml:patches").child("gml:PolygonPatch").child("gml:interior");
	while (nodeInterior)
	{
		std::string interiorCurveMemberID = nodeInterior.child("gml:Ring").child("gml:curveMember").attribute("xlink:href").value();

		if (interiorCurveMemberID.length() > 1)
		{
			interiorCurveMemberID = interiorCurveMemberID.substr(1);
		}
		else
		{
			break;
		}

		object->AddInteriorRingID(interiorCurveMemberID);
		nodeInterior = nodeInterior.next_sibling("gml:interior");
	}

	return true; 
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
		complexAttribute->AddSubSimpleAttribute(sa->GetValueType(), value);
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