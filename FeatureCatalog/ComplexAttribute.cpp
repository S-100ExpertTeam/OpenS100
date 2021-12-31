#include "stdafx.h"
#include "ComplexAttribute.h"

ComplexAttribute::ComplexAttribute()
{

}

ComplexAttribute::~ComplexAttribute()
{

}

void ComplexAttribute::GetContents(pugi::xml_node& node)
{
	((Item*)this)->Item::GetContents(node);
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:subAttributeBinding"))
		{
			subAttributeBinding.push_back(AttributeBinding());
			subAttributeBinding.back().GetContents(instruction);
		}
	}
}

void ComplexAttribute::SetSubAttributeBinding(AttributeBinding value)
{
	subAttributeBinding.push_back(value);
}

void ComplexAttribute::SetSubAttributeBinding(std::list<AttributeBinding> value)
{
	subAttributeBinding = value;
}

AttributeBinding ComplexAttribute::GetSubAttributeBinding(int index)
{
	auto it = subAttributeBinding.begin();
	std::advance(it, index);
	return *it;
}

std::list<AttributeBinding>& ComplexAttribute::GetSubAttributeBindingPointer()
{
	return subAttributeBinding;
}