#include "stdafx.h"
#include "Reference.h"

Reference::Reference()
{

}

Reference::~Reference()
{

}

void Reference::GetContents(pugi::xml_node& node)
{
	for (auto attri =node.first_attribute(); attri; attri =attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"ref"))
		{
			XML_Attribute attr;
			attr.Setname("ref");
			attr.Setvalue((char*)node.attribute("ref").value());
			SetAttributes(attr);
		}
	}
}