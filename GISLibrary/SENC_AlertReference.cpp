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

void SENC_AlertReference::setPlan(const std::string& plan)
{
	this->plan = plan;
}

std::optional<std::string> SENC_AlertReference::getPlan() const
{
	return plan;
}

void SENC_AlertReference::setMonitoring(const std::string& monitoring)
{
	this->monitoring = monitoring;
}

std::optional<std::string> SENC_AlertReference::getMonitoring() const
{
	return monitoring;
}
