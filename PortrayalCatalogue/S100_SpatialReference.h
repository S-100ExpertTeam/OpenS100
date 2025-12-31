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
	//std::wstring type;

	// ex : 3 
	// Curve RCID
	std::string reference;

	// boolean
	S100::Boolean forward = true;
public:
	//void SetType(std::string& value);
	//void SetType(std::string_view value);
	void SetReference(std::string& value);
	void SetReference(std::string_view value);
	void SetForward();
	void SetBackward();

	//std::wstring GetType();
	std::string GetReference();
	std::wstring GetReferenceAsWString();
	bool isForward();
	bool isBackward();

	// value : reference[,forward]
	void Set(std::string& value);
};