#pragma once

#include "../FeatureCatalog/PrimitiveTypes.h"

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
	S100::Boolean forward = true;
public:
	void SetType(std::string &value);
	void SetReference(std::string &value);
	void SetForward();
	void SetBackward();

	std::wstring GetType();
	std::wstring GetReference();
	bool isForward();
	bool isBackward();
};