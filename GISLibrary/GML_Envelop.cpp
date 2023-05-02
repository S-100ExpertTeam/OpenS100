#include "stdafx.h"
#include "GML_Envelop.h"

namespace S100GML
{
	Envelop::Envelop()
	{

	}

	Envelop::Envelop(const MBR& mbr)
	{
		this->mbr = mbr;
		inverseProjection(this->mbr);
	}

	Envelop::~Envelop()
	{

	}

	void Envelop::Read(pugi::xml_node& node)
	{

	}

	void Envelop::Write(pugi::xml_node& node)
	{
		auto node_boundedBy = node.append_child("gml:boundedBy");
		auto node_Envelope = node_boundedBy.append_child("gml:Envelope");
		node_Envelope.append_attribute("srsName").set_value("http://www.opengis.net/def/crs/EPSG/0/4326");
		auto node_lowerCorner = node_Envelope.append_child("gml:lowerCorner");
		auto node_upperCorner = node_Envelope.append_child("gml:upperCorner");
		node_lowerCorner.append_child(pugi::node_pcdata).set_value(GetLowerCornerString().c_str());
		node_upperCorner.append_child(pugi::node_pcdata).set_value(GetUpperCornerString().c_str());
	}

	std::string Envelop::GetLowerCornerString()
	{
		return std::to_string(mbr.GetYMin()) + " " + std::to_string(mbr.GetXMin());
	}

	std::string Envelop::GetUpperCornerString()
	{
		return std::to_string(mbr.GetYMax()) + " " + std::to_string(mbr.GetXMax());
	}
}