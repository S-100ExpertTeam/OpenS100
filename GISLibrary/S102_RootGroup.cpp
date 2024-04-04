#include "stdafx.h"
#include "S102_RootGroup.h"
#include "HDF5Wrapper.h"

S102_RootGroup::S102_RootGroup()
{

}

S102_RootGroup::S102_RootGroup(const S102_RootGroup& other) : H5_RootGroup(other)
{
	setGriddingMethod(*other.griddingMethod);
}

S102_RootGroup::~S102_RootGroup()
{
	delete griddingMethod;
}

S102_RootGroup S102_RootGroup::operator=(const S102_RootGroup& other)
{
	H5_RootGroup::operator=(other);

	setGriddingMethod(*other.griddingMethod);

	return *this;
}

bool S102_RootGroup::hasGriddingMethod() const
{
	return griddingMethod;
}

S102_GriddingMethod S102_RootGroup::getGriddingMethod() const
{
	if (griddingMethod) {
		return *griddingMethod;
	}

	return S102_GriddingMethod::none;
}

void S102_RootGroup::setGriddingMethod(const S102_GriddingMethod value)
{
	if (!griddingMethod)
	{
		griddingMethod = new S102_GriddingMethod();
	}

	*griddingMethod = value;
}

bool S102_RootGroup::Read(hid_t rootID)
{
	H5_RootGroup::Read(rootID);

	int iGriddingMethod = 0;
	if (HDF5Wrapper::ReadMetadataIntegerAttribute(rootID, "griddingMethod", iGriddingMethod)) {
		setGriddingMethod((S102_GriddingMethod)iGriddingMethod);
	}

	return true;
}