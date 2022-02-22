#include "stdafx.h"
#include "S100_Transform.h"

S100_Transform::S100_Transform()
{
	isTransfer = false;
}

S100_Transform::~S100_Transform()
{
	if (!isTransfer)
	{
		for (auto itor = templates.begin(); itor != templates.end(); itor++)
			delete *itor;
	}

	for (auto itor = includes.begin(); itor != includes.end(); itor++)
		delete *itor;
}

void S100_Transform::CopyTo(S100_Transform* toObject)
{
	toObject->templates.insert(toObject->templates.end(), templates.begin(), templates.end());
	isTransfer = true;
}

void S100_Transform::SetIsTransfer(bool value)
{
	isTransfer = value;
}

bool S100_Transform::GetIsTransfer() 
{
	return isTransfer;
}

void S100_Transform::SetTemplates(S100_Template* value) 
{
	templates.push_back(value);
}

void S100_Transform::SetTemplates(std::vector<S100_Template*> value) 
{
	 templates = value;
}

void S100_Transform::SetInclude(S100_Include* value) 
{
	includes.push_back(value);
}

void S100_Transform::SetIncludes(std::vector<S100_Include*> value) 
{
	includes = value;
}

S100_Template* S100_Transform::GetTemplate(int index)
{
	return templates.at(index);
}
std::vector<S100_Template*> S100_Transform::GetTemplates() 
{
	return templates;
}

S100_Include* S100_Transform::GetInclude(int index)
{
	return includes.at(index);
}

std::vector<S100_Include*> S100_Transform::GetIncludes()
{
	return includes;
}
