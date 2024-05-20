#pragma once

#include "PugixmlElement.h"
#include "S100GML_NameSpace.h"

#include "../GeoMetryLibrary/MBR.h"

namespace S100GML
{

	class Envelop : 
		public PugixmlElement
	{
	public:
		Envelop();
		Envelop(const MBR& mbr);
		virtual ~Envelop();

	public:
		MBR mbr; // unit : WGS84
		std::string srsName;

	public:
		void Read(pugi::xml_node& node) override;
		void Write(pugi::xml_node& node) override; 

		std::string GetLowerCornerString();
		std::string GetUpperCornerString();

		void setSrsName(std::string& srsName);
		std::string getSrsName();

		bool isXYSequence();
		bool isYXSequence();


	// for gml namespace
		void SetNamespace(S100GML_NameSpace ns);

	private:
		S100GML_NameSpace s100namespace;
	};

}