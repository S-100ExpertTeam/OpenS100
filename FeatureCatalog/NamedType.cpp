#include "stdafx.h"
#include "NamedType.h"

NamedType::NamedType()
{

}

NamedType::~NamedType()
{

}

void NamedType::GetContents(pugi::xml_node& node)
{
	this->Item::GetContents(node);

	if (node.attribute("isAbstract"))
	{
		XML_Attribute attr;
		attr.Setname("isAbstract");
		attr.Setvalue((char*)node.attribute("isAbstract").value());
		SetAttributes(attr);
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:attributeBinding"))
		{
			attributeBinding.push_back(AttributeBinding());
			attributeBinding.back().GetContents(instruction);
		}
	}
}

std::list<AttributeBinding>& NamedType::GetAttributeBindingPointer()
{
	return attributeBinding;
}