#include "stdafx.h"
#include "S100_AlertReference.h"

S100_AlertReference::S100_AlertReference()
{
}

S100_AlertReference::~S100_AlertReference()
{
}

void S100_AlertReference::setReference(std::string value)
{
	reference = value;
}

void S100_AlertReference::setPlan(std::string value)
{
	plan = value;
}

void S100_AlertReference::setMonitor(std::string value)
{
	monitor = value;
}

S100::CharacterString S100_AlertReference::getReference()
{
	return reference;
}

std::optional<S100::CharacterString> S100_AlertReference::getPlan()
{
	if (plan.has_value())
	{
		return plan.value();
	}

	return S100::CharacterString();
}

std::optional<S100::CharacterString> S100_AlertReference::getMonitor()
{
	if (monitor.has_value())
	{
		return monitor.value();
	}

	return S100::CharacterString();
}