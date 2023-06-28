#include "stdafx.h"
#include "S100H5.h"
#include "S102_RootMeta.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include <hdf5.h>

S100H5::S100H5()
{
	m_FileType = FILE_S_100_GRID_H5;
}

S100H5::~S100H5()
{
	delete rootMeta;
}

bool S100H5::Open(CString _filepath)
{
	auto path = LibMFCUtil::WStringToString(std::wstring(_filepath));

	auto fileID = H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	if (fileID < 0)
	{
		return false;
	}

	if (GetFC()->getProductId().compare("S-102") == 0)
	{
		rootMeta = new S102_RootMeta();
	}
	else
	{
		rootMeta = new H5_RootMeta();
	}
	
	rootMeta->Read(fileID);

	H5Fclose(fileID);

	SetMBR();

	return true;
}

void S100H5::SetMBR()
{
	auto layer = GetLayer();
	if (layer && rootMeta)
	{
		MBR mbr;
		mbr.SetXMin(rootMeta->getWestBoundLongitude());
		mbr.SetYMin(rootMeta->getSouthBoundLatitude());
		mbr.SetXMax(rootMeta->getEastBoundLongitude());
		mbr.SetYMax(rootMeta->getNorthBoundLatitude());
		projection(mbr);
		layer->SetMBR(mbr);
	}
}