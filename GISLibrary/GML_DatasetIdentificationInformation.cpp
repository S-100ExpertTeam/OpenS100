#include "stdafx.h"
#include "GML_DatasetIdentificationInformation.h"

namespace S100GML
{

	DatasetIdentificationInformation::DatasetIdentificationInformation()
	{

	}

	DatasetIdentificationInformation::~DatasetIdentificationInformation()
	{

	}

	void DatasetIdentificationInformation::Read(pugi::xml_node& node)
	{

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