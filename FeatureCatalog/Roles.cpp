#include "stdafx.h"
#include "Roles.h"

#include "../LibMFCUtil/LibMFCUtil.h"

Roles::Roles(void)
{

}

Roles::~Roles(void)
{
	for (auto i = role.begin(); i != role.end(); i++)
	{
		delete i->second;
	}
	role.clear();
}

void Roles::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_Role"))
		{
			Role* r = new Role();
			r->GetContents(instruction);
			role.insert(std::make_pair(r->GetCodeAsWString(), r));
		}
	}
}

std::unordered_map<std::wstring, Role*>& Roles::GetRolePointer()
{
	return role;
}
