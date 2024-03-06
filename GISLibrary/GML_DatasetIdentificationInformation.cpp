#include "stdafx.h"
#include "GML_DatasetIdentificationInformation.h"

namespace S100GML
{

	DatasetIdentificationInformation::DatasetIdentificationInformation()
	{

	}

	DatasetIdentificationInformation::DatasetIdentificationInformation(const DatasetIdentificationInformation& dii)
	{
		encodingSpecification = dii.encodingSpecification;
		encodingSpecificationEdition = dii.encodingSpecificationEdition;
		productIdentifier = dii.productIdentifier;
		productEdition = dii.productEdition;
		applicationProfile = dii.applicationProfile;
		datasetFileIdentifier = dii.datasetFileIdentifier;
		datasetTitle = dii.datasetTitle;
		datasetReferenceDate = dii.datasetReferenceDate;
		datasetLanguage = dii.datasetLanguage;
		for (const auto& iter : dii.datasetTopicCategory)
			datasetTopicCategory.push_back(iter);
		datasetPurpose = dii.datasetPurpose;
		updateNumber = dii.updateNumber;
	}

	DatasetIdentificationInformation::~DatasetIdentificationInformation()
	{

	}

	void DatasetIdentificationInformation::Read(pugi::xml_node& node)
	{
		encodingSpecification = node.child_value("S100:encodingSpecification");
		encodingSpecificationEdition = node.child_value("S100:encodingSpecificationEdition");
		productIdentifier = node.child_value("S100:productIdentifier");
		productEdition = node.child_value("S100:productEdition");
		applicationProfile = node.child_value("S100:applicationProfile");
		datasetFileIdentifier = node.child_value("S100:datasetFileIdentifier");
		datasetTitle = node.child_value("S100:datasetTitle");
		datasetReferenceDate = node.child_value("S100:datasetReferenceDate");
		datasetLanguage = node.child_value("S100:datasetLanguage");

		auto node_datasetTopicCategory = node.child("S100:datasetTopicCategory");
		while (node_datasetTopicCategory)
		{
			datasetTopicCategory.push_back(node_datasetTopicCategory.child_value());
			node_datasetTopicCategory = node_datasetTopicCategory.next_sibling("S100:datasetTopicCategory");
		}

		datasetPurpose = node.child_value("S100:datasetPurpose");
		updateNumber = node.child_value("S100:updateNumber");
	}

	void DatasetIdentificationInformation::Write(pugi::xml_node& node)
	{
		auto curNode = node.append_child("S100:DatasetIdentificationInformation");
		curNode.append_child("S100:encodingSpecification").append_child(pugi::node_pcdata).set_value(encodingSpecification.c_str());
		curNode.append_child("S100:encodingSpecificationEdition").append_child(pugi::node_pcdata).set_value(encodingSpecificationEdition.c_str());
		curNode.append_child("S100:productIdentifier").append_child(pugi::node_pcdata).set_value(productIdentifier.c_str());
		curNode.append_child("S100:productEdition").append_child(pugi::node_pcdata).set_value(productEdition.c_str());
		curNode.append_child("S100:applicationProfile").append_child(pugi::node_pcdata).set_value(applicationProfile.c_str());
		curNode.append_child("S100:datasetFileIdentifier").append_child(pugi::node_pcdata).set_value(datasetFileIdentifier.c_str());
		curNode.append_child("S100:datasetTitle").append_child(pugi::node_pcdata).set_value(datasetTitle.c_str());
		curNode.append_child("S100:datasetReferenceDate").append_child(pugi::node_pcdata).set_value(datasetReferenceDate.c_str());
		curNode.append_child("S100:datasetLanguage").append_child(pugi::node_pcdata).set_value(datasetLanguage.c_str());

		for (auto i = datasetTopicCategory.begin(); i != datasetTopicCategory.end(); i++)
		{
			curNode.append_child("S100:datasetTopicCategory").append_child(pugi::node_pcdata).set_value((*i).c_str());
		}

		curNode.append_child("S100:datasetPurpose").append_child(pugi::node_pcdata).set_value(datasetPurpose.c_str());
		curNode.append_child("S100:updateNumber").append_child(pugi::node_pcdata).set_value(updateNumber.c_str());
	}
}