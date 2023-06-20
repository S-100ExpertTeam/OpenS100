#pragma once

#include "PugixmlElement.h"

#include <string>
#include <vector>

namespace S100GML
{

	class DatasetIdentificationInformation :
		public PugixmlElement
	{
	public:
		DatasetIdentificationInformation();
		virtual ~DatasetIdentificationInformation();

	public:
		std::string encodingSpecification;
		std::string encodingSpecificationEdition;
		std::string productIdentifier;
		std::string productEdition;
		std::string applicationProfile;
		std::string datasetFileIdentifier;
		std::string datasetTitle;
		std::string datasetReferenceDate;
		std::string datasetLanguage;
		std::vector<std::string> datasetTopicCategory;
		std::string datasetPurpose;
		std::string updateNumber;

	public:
		void Read(pugi::xml_node& node) override;
		void Write(pugi::xml_node& node) override;
	};

}