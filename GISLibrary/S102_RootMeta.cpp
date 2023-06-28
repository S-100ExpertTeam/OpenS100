#include "stdafx.h"
#include "S102_RootMeta.h"
#include "HDF5Wrapper.h"

S102_RootMeta::S102_RootMeta()
{

}

S102_RootMeta::~S102_RootMeta()
{
	delete griddingMethod;
}

bool S102_RootMeta::hasGriddingMethod() const
{
	return griddingMethod;
}

S102_GriddingMethod S102_RootMeta::getGriddingMethod() const
{
	return *griddingMethod;
}

void S102_RootMeta::setGriddingMethod(const S102_GriddingMethod value)
{
	if (!griddingMethod)
	{
		griddingMethod = new S102_GriddingMethod();
	}

	*griddingMethod = value;
}

bool S102_RootMeta::Read(hid_t rootID)
{
	H5_RootMeta::Read(rootID);

	int iGriddingMethod = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "griddingMethod", iGriddingMethod)) {
		setGriddingMethod((S102_GriddingMethod)iGriddingMethod);
	}

	return true;
}