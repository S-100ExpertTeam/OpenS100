#include "stdafx.h"
#include "S100H5.h"
#include "S102_RootGroup.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <hdf5.h>

S100H5::S100H5()
{
	type = S100SpatialObjectType::S100H5;
	m_FileType = S100_FileType::FILE_S_100_GRID_H5;
}

S100H5::~S100H5()
{
	delete rootGroup;

	for (auto i = featureContainer.begin(); i != featureContainer.end(); i++)
	{
		delete (*i);
	}
}

bool S100H5::Open(CString _filepath)
{
	SetFilePath(_filepath);
	SetFileName(LibMFCUtil::GetFileName(_filepath));

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