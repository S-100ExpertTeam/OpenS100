#pragma once
#include "S100_AreaFillBase.h"

#include <pugixml.hpp>

class S100_AreaFillReference : public S100_AreaFillBase
{
public:
	S100_AreaFillReference();
	virtual ~S100_AreaFillReference();

private:
	std::string reference;

public:
	void GetContents(pugi::xml_node node);

	void SetReference(const std::string& value);
	void SetReference(const std::wstring& value);
	std::string  GetReference();
	std::wstring GetReferenceW();
};
