#include "stdafx.h"
#include "S10XGML.h"
#include "GM_Point.h"
#include "GF_FeatureType.h"
#include "GF_InformationType.h"

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
	features.push_back(feature);

	auto child = node.first_child();
	while (child)
	{
		std::string childName = child.name();

		auto attribute = fc->GetAttribute(childName);
		if (attribute)
		{

		}
		else if (childName.compare("geometry") == 0)
		{
			
		}
		else
		{
			auto role = fc->GetRole(childName);
			if (role)
			{

			}
		}

		child = child.next_sibling();
	}
	return true;
}

bool S10XGML::ReadInformation(pugi::xml_node& node, FeatureCatalogue* fc, InformationType* informationType)
{
	return true;
}