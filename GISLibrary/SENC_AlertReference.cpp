#include "stdafx.h"
#include "SENC_AlertReference.h"

SENC_AlertReference::SENC_AlertReference()
{

}

SENC_AlertReference::~SENC_AlertReference()
{

}

void SENC_AlertReference::setReference(const std::string& ref)
{
	reference = ref;
}

std::string SENC_AlertReference::getReference() const
{
	return reference;
}

void SENC_AlertReference::setPlan(const std::string& _plan)
{
	this->plan = _plan;
}

std::optional<std::string> SENC_AlertReference::getPlan() const
{
	return plan;
}

void SENC_AlertReference::setMonitoring(const std::string& _monitoring)
{
	this->monitoring = _monitoring;
}

std::optional<std::string> SENC_AlertReference::getMonitoring() const
{
	return monitoring;
}
