#include "stdafx.h"
#include "S100ObjectType.h"

S100ObjectType::S100ObjectType()
{

}

S100ObjectType::~S100ObjectType()
{

}

void S100ObjectType::GetContents(pugi::xml_node& node)
{
	this->NamedType::GetContents(node);

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:informationBinding"))
		{
			InformationBinding ib;
			ib.GetContents(instruction);

			std::wstring associatename = ib.GetInformationTypePointer().Getvalue();
			if (associatename.compare(L"") == 0)
			{
				std::list<XML_Attribute> attributeList = ib.GetInformationTypePointer().GetattributesPointer();

				for (auto itor = attributeList.begin();
					itor != attributeList.end();
					itor++)
				{
					XML_Attribute* attr = &(*itor);
					if (attr->Getname().compare(L"ref") == 0)
					{
						associatename = attr->Getvalue();
					}
				}
			}
			informationBinding[associatename] = ib;
		}
	}
}

std::unordered_map<std::wstring, InformationBinding>& S100ObjectType::GetInformationBindingPointer()
{
	return informationBinding;
}