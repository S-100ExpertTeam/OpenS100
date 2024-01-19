#include "stdafx.h"
#include "HDF5Wrapper.h"

#include <hdf5.h>


HDF5Wrapper::HDF5Wrapper()
{
}


HDF5Wrapper::~HDF5Wrapper()
{
}


bool HDF5Wrapper::ReadMetadataIntegerAttribute(__int64 id, char* attributeName, int& attributeValue)
{
	if (IsInteger64Attribute(id))
	{
		long long int64Value = 0;
		auto result = ReadMetadataInteger64Attribute(id, attributeName, int64Value);
		attributeValue = (int)int64Value;
		return result;
	}

	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	status = H5Aread(id_attribute, H5T_NATIVE_INT, &attributeValue);

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}


bool HDF5Wrapper::ReadMetadataInteger64Attribute(__int64 id, char* attributeName, __int64& attributeValue)
{
	if (IsInteger32Attribute(id))
	{
		int int32Value = 0;
		auto result = ReadMetadataIntegerAttribute(id, attributeName, int32Value);
		attributeValue = int32Value;
		return result;
	}

	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	status = H5Aread(id_attribute, H5T_NATIVE_INT64, &attributeValue);

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}



bool HDF5Wrapper::ReadMetadataFloatAttribute(__int64 id, char* attributeName, float& attributeValue)
{
	if (IsFloat64Attribute(id))
	{
		double doubleValue = 0;
		auto result = ReadMetadataDoubleAttribute(id, attributeName, doubleValue);
		attributeValue = (float)doubleValue;
		return result;
	}

	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	status = H5Aread(id_attribute, H5T_IEEE_F32LE, &attributeValue);

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}


bool HDF5Wrapper::ReadMetadataDoubleAttribute(__int64 id, char* attributeName, double& attributeValue)
{
	if (IsFloat32Attribute(id))
	{
		float floatValue = 0;
		auto result = ReadMetadataFloatAttribute(id, attributeName, floatValue);
		attributeValue = floatValue;
		return result;
	}

	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	status = H5Aread(id_attribute, H5T_IEEE_F64LE, &attributeValue);

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}



bool HDF5Wrapper::ReadMetadataStringAttribute(__int64 id, char* attributeName, std::string& attributeValue)
{
	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	hid_t type = H5Aget_type(id_attribute);

	if (type < 0 || H5Tis_variable_str(type) > 0)
	{
		H5Aclose(id_attribute);
		return false;
	}

	auto size = H5Aget_storage_size(id_attribute);

	char* buf = new char[(unsigned int)(size + 1)];
	memset(buf, 0, (size_t)(sizeof(char) * (size + 1)));

	hid_t type_class = H5Tget_class(type);

	if (type_class != H5T_STRING)
	{
		H5Tclose(type);
		H5Aclose(id_attribute);
		return false;
	}

	hid_t type_mem = H5Tget_native_type(type, H5T_DIR_ASCEND);

	if (type_mem < 0)
	{
		H5Tclose(type);
		H5Aclose(id_attribute);
		return false;
	}

	status = H5Aread(id_attribute, type_mem, buf);

	if (status != 0)
	{
		H5Tclose(type);
		H5Aclose(id_attribute);
		return false;
	}

	status = H5Tclose(type);

	if (status != 0)
	{
		H5Aclose(id_attribute);
		return false;
	}

	attributeValue = buf;
	delete[] buf;

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}


bool HDF5Wrapper::ReadMetadataVariableLengthStringAttribute(__int64 id, char* attributeName, std::string& attributeValue)
{
	herr_t status = 0;

	hid_t id_attribute = H5Aopen_name(id, attributeName);

	if (id_attribute < 0)
	{
		return false;
	}

	hid_t type = H5Aget_type(id_attribute);

	if (H5T_VLEN != H5Tget_class(type))
	{
		H5Tclose(type);
		H5Aclose(id_attribute);
		return false;
	}

	if (type < 0)
	{
		H5Aclose(id_attribute);
		return false;
	}

	H5Tset_size(type, H5T_VARIABLE);

	char* vbuf = nullptr;

	status = H5Aread(id_attribute, type, &vbuf);

	if (status < 0)
	{
		delete[] vbuf;
		H5Tclose(type);
		H5Aclose(id_attribute);
		return false;
	}

	status = H5Tclose(type);

	if (status != 0)
	{
		H5Aclose(id_attribute);
		return false;
	}

	attributeValue = vbuf;
	//delete[] vbuf;

	if (status != 0)
	{
		H5Aclose(id_attribute);

		return false;
	}

	status = H5Aclose(id_attribute);

	if (status != 0)
	{
		return false;
	}

	return true;
}

bool HDF5Wrapper::ReadMetadataAnyStringAttribute(__int64 id, char* attributeName, std::string& attributeValue)
{
	if (!ReadMetadataStringAttribute(id, attributeName, attributeValue))
	{
		return ReadMetadataVariableLengthStringAttribute(id, attributeName, attributeValue);
	}

	return true;
}

bool HDF5Wrapper::IsFloat32Attribute(__int64 id)
{
	auto type = H5Aget_type(id);

	auto is32bit = H5Tequal(type, H5T_IEEE_F32LE);

	if (is32bit > 0)
	{
		return true;
	}
	
	return false;
}

bool HDF5Wrapper::IsFloat64Attribute(__int64 id)
{
	auto type = H5Aget_type(id);

	auto is64bit = H5Tequal(type, H5T_IEEE_F64LE);

	if (is64bit > 0)
	{
		return true;
	}

	return false;
}

bool HDF5Wrapper::IsInteger32Attribute(__int64 id)
{
	auto type = H5Aget_type(id);

	auto is32bit = H5Tequal(type, H5T_NATIVE_INT);

	if (is32bit > 0)
	{
		return true;
	}

	return false;
}

bool HDF5Wrapper::IsInteger64Attribute(__int64 id)
{
	auto type = H5Aget_type(id);

	auto is64bit = H5Tequal(type, H5T_NATIVE_INT64);

	if (is64bit > 0)
	{
		return true;
	}

	return false;
}