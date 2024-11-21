#pragma once
#include "S100_AreaFillBase.h"

#include <pugixml.hpp>

class S100_AreaFillReference : public S100_AreaFillBase
{
public:
	S100_AreaFillReference();
	virtual ~S100_AreaFillReference();

private:
	std::wstring reference;

public:
	void GetContents(pugi::xml_node node);

	void SetReference(std::wstring& value);
	void SetReference(std::string& value);
	std::wstring GetReference();
};

