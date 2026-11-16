#pragma once

#include <string>

class HDF5Wrapper
{
public:
	HDF5Wrapper();
	virtual ~HDF5Wrapper();

	static bool ReadMetadataIntegerAttribute(long long id, const char* attributeName, int& attributeValue);
	static bool ReadMetadataInteger64Attribute(long long id, const char* attributeName, long long& attributeValue);
	static bool ReadMetadataFloatAttribute(long long id, const char* attributeName, float& attributeValue);
	static bool ReadMetadataDoubleAttribute(long long id, const char* attributeName, double& attributeValue);
	static bool ReadMetadataStringAttribute(long long id, const char* attributeName, std::string& attributeValue);
	static bool ReadMetadataVariableLengthStringAttribute(long long id, const char* attributeName, std::string& attributeValue);
	static bool ReadMetadataAnyStringAttribute(long long id, const char* attributeName, std::string& attributeValue);

	static bool IsFloat32Attribute(long long id);
	static bool IsFloat64Attribute(long long id);
	static bool IsInteger32Attribute(long long id);
	static bool IsInteger64Attribute(long long id);
};