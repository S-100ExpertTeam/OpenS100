#pragma once
#include <string>

class S100_SpatialReference
{
public:
	S100_SpatialReference();
	virtual ~S100_SpatialReference();
private:
	// ex : Curve
	std::wstring type;
	// ex : 3 
	// Curve RCID
	std::wstring reference;
	// boolean
	std::wstring forward;
public:
	void SetType(std::string &value);
	void SetReference(std::string &value);
	void SetForward(std::string &value);

	std::wstring GetType();
	std::wstring GetReference();
	std::wstring GetForward();
};