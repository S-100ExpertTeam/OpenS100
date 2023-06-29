#include "stdafx.h"
#include "S102H5.h"
#include "S102_RootGroup.h"
#include "S102_FC_BathymetryCoverage.h"

#include "..\LibMFCUtil\LibMFCUtil.h"

#include <hdf5.h>

S102H5::S102H5()
{
	featureContainer.push_back(new S102_FC_BathymetryCoverage());
}

S102H5::~S102H5()
{
	
}

bool S102H5::Open(CString _filepath)
{
	auto path = LibMFCUtil::WStringToString(std::wstring(_filepath));

	auto fileID = H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	if (fileID < 0)
	{
		return false;
	}

	rootGroup = new S102_RootGroup();

	rootGroup->Read(fileID);

	auto bathymetryCoverage = GetBathymetryCoverage();
	if (bathymetryCoverage)
	{
		auto groupID = H5Gopen(fileID, "BathymetryCoverage", H5P_DEFAULT);
		if (groupID > 0)
		{
			if (!bathymetryCoverage->Read(groupID))
			{
				H5Fclose(fileID);
				return false;
			}
		}
		else
		{
			H5Fclose(fileID);
			return false;
		}
	}

	H5Fclose(fileID);

	SetMBR();

	return true;
}

S102_FC_BathymetryCoverage* S102H5::GetBathymetryCoverage()
{
	return (S102_FC_BathymetryCoverage*)featureContainer.at(0);
}