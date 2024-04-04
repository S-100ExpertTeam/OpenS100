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
		encodingSpecification = node.child_value((s100namespace.namespace_s100 + ":encodingSpecification").c_str());
		encodingSpecificationEdition = node.child_value((s100namespace.namespace_s100 + ":encodingSpecificationEdition").c_str());
		productIdentifier = node.child_value((s100namespace.namespace_s100 + ":productIdentifier").c_str());
		productEdition = node.child_value((s100namespace.namespace_s100 + ":productEdition").c_str());
		applicationProfile = node.child_value((s100namespace.namespace_s100 + ":applicationProfile").c_str());
		datasetFileIdentifier = node.child_value((s100namespace.namespace_s100 + ":datasetFileIdentifier").c_str());
		datasetTitle = node.child_value((s100namespace.namespace_s100 + ":datasetTitle").c_str());
		datasetReferenceDate = node.child_value((s100namespace.namespace_s100 + ":datasetReferenceDate").c_str());
		datasetLanguage = node.child_value((s100namespace.namespace_s100 + ":datasetLanguage").c_str());

		auto node_datasetTopicCategory = node.child((s100namespace.namespace_s100 + ":datasetTopicCategory").c_str());
		while (node_datasetTopicCategory)
		{
			datasetTopicCategory.push_back(node_datasetTopicCategory.child_value());
			node_datasetTopicCategory = node_datasetTopicCategory.next_sibling((s100namespace.namespace_s100 + ":datasetTopicCategory").c_str());
		}

		datasetPurpose = node.child_value((s100namespace.namespace_s100 + ":datasetPurpose").c_str());
		updateNumber = node.child_value((s100namespace.namespace_s100 + ":updateNumber").c_str());
	}

	void DatasetIdentificationInformation::Write(pugi::xml_node& node)
	{
		auto curNode = node.append_child((s100namespace.namespace_s100 + ":DatasetIdentificationInformation").c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":encodingSpecification").c_str()).append_child(pugi::node_pcdata).set_value(encodingSpecification.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":encodingSpecificationEdition").c_str()).append_child(pugi::node_pcdata).set_value(encodingSpecificationEdition.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":productIdentifier").c_str()).append_child(pugi::node_pcdata).set_value(productIdentifier.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":productEdition").c_str()).append_child(pugi::node_pcdata).set_value(productEdition.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":applicationProfile").c_str()).append_child(pugi::node_pcdata).set_value(applicationProfile.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":datasetFileIdentifier").c_str()).append_child(pugi::node_pcdata).set_value(datasetFileIdentifier.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":datasetTitle").c_str()).append_child(pugi::node_pcdata).set_value(datasetTitle.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":datasetReferenceDate").c_str()).append_child(pugi::node_pcdata).set_value(datasetReferenceDate.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":datasetLanguage").c_str()).append_child(pugi::node_pcdata).set_value(datasetLanguage.c_str());

		for (auto i = datasetTopicCategory.begin(); i != datasetTopicCategory.end(); i++)
		{
			curNode.append_child((s100namespace.namespace_s100 + ":datasetTopicCategory").c_str()).append_child(pugi::node_pcdata).set_value((*i).c_str());
		}

		curNode.append_child((s100namespace.namespace_s100 + ":datasetPurpose").c_str()).append_child(pugi::node_pcdata).set_value(datasetPurpose.c_str());
		curNode.append_child((s100namespace.namespace_s100 + ":updateNumber").c_str()).append_child(pugi::node_pcdata).set_value(updateNumber.c_str());
	}

	void DatasetIdentificationInformation::SetNamespace(S100GML_NameSpace ns)
	{
		s100namespace = ns;
	}
}