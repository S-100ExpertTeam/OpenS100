#pragma once

#include <vector>

class LuaAttributePathItem
{
public:
	LuaAttributePathItem();
	virtual ~LuaAttributePathItem();

protected:
	std::string code;
	int index;
};

