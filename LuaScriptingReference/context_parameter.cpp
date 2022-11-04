#include "stdafx.h"
#include "context_parameter.h"

context_parameter::context_parameter()
{

}

context_parameter::context_parameter(std::string_view name, std::string_view type, std::string_view default_value)
{
	this->name = name;
	this->type = type;
	this->default_value = default_value;
}

context_parameter::~context_parameter()
{

}