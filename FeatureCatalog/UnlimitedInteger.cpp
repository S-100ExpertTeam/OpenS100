#include "stdafx.h"
#include "UnlimitedInteger.h"

#include <string>

UnlimitedInteger::UnlimitedInteger()
{

}

UnlimitedInteger::~UnlimitedInteger()
{

}

void UnlimitedInteger::GetContents(pugi::xml_node& node)
{
	std::wstring name = pugi::as_wide(node.name());

	if (node.attribute("infinite")) 
	{
		XML_Attribute value;
		value.Setname("infinite");
		value.Setvalue((char*)node.attribute("infinite").value());
		
		SetAttributes(value);
	}
	this->Setvalue(pugi::as_wide(node.child_value()));
}