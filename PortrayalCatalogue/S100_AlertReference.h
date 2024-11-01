#pragma once

#include "../FeatureCatalog/PrimitiveTypes.h"

#include <string>
#include <optional>

class S100_AlertReference
{
public:
	S100_AlertReference();
	virtual ~S100_AlertReference();

protected:
	S100::CharacterString reference;
	std::optional<S100::CharacterString> plan;
	std::optional<S100::CharacterString> monitor;

public:
	void setReference(std::string value);
	void setPlan(std::string value);
	void setMonitor(std::string value);

	S100::CharacterString getReference();
	std::optional<S100::CharacterString> getPlan();
	std::optional<S100::CharacterString> getMonitor();
};
