#pragma once

#include "../GISLibrary/GF_FeatureType.h"

#include <string>

class R_FeatureRecord;

class CFeatureCodeString
{
public:
	int _id; 
	std::wstring _name;
	//R_FeatureRecord* _fr;
	GF::FeatureType* _fr = nullptr;

public:
	//Operator overloading, does not work before sort.
	bool operator < (CFeatureCodeString& value);
};