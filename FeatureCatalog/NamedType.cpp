#include "stdafx.h"
#include "NamedType.h"

NamedType::NamedType()
{

}

NamedType::~NamedType()
{
	for (auto i = attributeBinding.begin(); i != attributeBinding.end(); i++)
	{
		delete (*i);
	}
}

void NamedType::GetContents(pugi::xml_node& node)
{
	this->Item::GetContents(node);

	if (auto attribute = node.attribute("isAbstract"))
	{
		auto attributeValue = attribute.as_bool();
		SetIsAbstract(attributeValue);
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:attributeBinding"))
		{
			auto attributeBinding = new AttributeBinding();
			attributeBinding->GetContents(instruction);
			this->attributeBinding.push_back(attributeBinding);
		}
	}
}

std::list<AttributeBinding*>& NamedType::GetAttributeBindingPointer()
{
	return attributeBinding;
}

std::list<AttributeBinding*>& NamedType::GetAttributeBindingList()
{
	return attributeBinding;
}

void NamedType::SetIsAbstract(bool value)
{
	isAbstract = value;
}

bool NamedType::GetIsAbstract()
{
	return isAbstract;
}