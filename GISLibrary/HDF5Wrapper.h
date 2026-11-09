#pragma once

#include <string>

class HDF5Wrapper
{
public:
	HDF5Wrapper();
	virtual ~HDF5Wrapper();

	static bool ReadMetadataIntegerAttribute(__int64 id, const char* attributeName, int& attributeValue);
	static bool ReadMetadataInteger64Attribute(__int64 id, const char* attributeName, __int64& attributeValue);
	static bool ReadMetadataFloatAttribute(__int64 id, const char* attributeName, float& attributeValue);
	static bool ReadMetadataDoubleAttribute(__int64 id, const char* attributeName, double& attributeValue);
	static bool ReadMetadataStringAttribute(__int64 id, const char* attributeName, std::string& attributeValue);
	static bool ReadMetadataVariableLengthStringAttribute(__int64 id, const char* attributeName, std::string& attributeValue);
	static bool ReadMetadataAnyStringAttribute(__int64 id, const char* attributeName, std::string& attributeValue);

	static bool IsFloat32Attribute(__int64 id);
	static bool IsFloat64Attribute(__int64 id);
	static bool IsInteger32Attribute(__int64 id);
	static bool IsInteger64Attribute(__int64 id);
};