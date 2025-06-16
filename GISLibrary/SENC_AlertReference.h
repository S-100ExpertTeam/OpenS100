#pragma once

class SENC_AlertReference
{
public:
	SENC_AlertReference();
	virtual ~SENC_AlertReference();

public:
	std::string reference;
	std::optional<std::string> plan;
	std::optional<std::string> monitoring;

public:
	void setReference(const std::string& ref);
	std::string getReference() const;

	void setPlan(const std::string& plan);
	std::optional<std::string> getPlan() const;

	void setMonitoring(const std::string& monitoring);
	std::optional<std::string> getMonitoring() const;
};