#include "stdafx.h"
#include "PathUtil.h"
#include "S100H5.h"
#include "S102_RootGroup.h"

#include <hdf5.h>

S100H5::S100H5() : S100SpatialObject()
{
	type = S100SpatialObjectType::S100H5;
	m_FileType = S100_FileType::FILE_S_100_GRID_H5;
	m_ObejctType = SpatialObjectType::S100H5;
}

S100H5::~S100H5()
{
	delete rootGroup;

	for (auto i = featureContainer.begin(); i != featureContainer.end(); i++)
	{
		delete (*i);
	}
}

bool S100H5::Open(const std::wstring& _filepath)
{
	SetFilePath(_filepath);
	SetFileName(s100::path::Stem(_filepath)); // HDF5 datasets are named by their stem

	auto path = pugi::as_utf8(_filepath);

	auto fileID = H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	if (fileID < 0)
	{
		return false;
	}

	rootGroup = new H5_RootGroup();
	
	rootGroup->Read(fileID);

	H5Fclose(fileID);

	SetMBR();

	return true;
}

bool S100H5::OpenMetadata(const std::wstring& /*_filepath*/)
{
	return true;
}

void S100H5::SetMBR()
{
	auto layer = GetLayer();
	if (layer && rootGroup)
	{
		MBR mbr;
		mbr.SetXMin(rootGroup->getWestBoundLongitude());
		mbr.SetYMin(rootGroup->getSouthBoundLatitude());
		mbr.SetXMax(rootGroup->getEastBoundLongitude());
		mbr.SetYMax(rootGroup->getNorthBoundLatitude());
		projection(mbr);
		layer->SetMBR(mbr);
	}
}