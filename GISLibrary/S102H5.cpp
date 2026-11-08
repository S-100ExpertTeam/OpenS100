#include "stdafx.h"
#include "S102H5.h"
#include "S102_RootGroup.h"
#include "S102_FC_BathymetryCoverage.h"
#include "GISLibrary.h"

#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GeoMetryLibrary/ENCCommon.h"

#include <hdf5.h>

S102H5::S102H5(PortrayalCatalogue* pc) : S100H5()
{
	type = S100SpatialObjectType::S102H5;
	featureContainer.push_back(new S102_FC_BathymetryCoverage());
	m_ObejctType = SpatialObjectType::S102H5;

	if (!s102Color.isSet()) {
		s102Color.setColor(pc);
	}
}

S102H5::~S102H5()
{
	DeleteBitmap();
}

bool S102H5::Open(CString _filepath)
{
	SetFilePath(_filepath);
	SetFileName(LibMFCUtil::GetFileName(_filepath));

	auto path = pugi::as_utf8(_filepath);

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

	SetPositive();

	H5Fclose(fileID);

	SetMBR();

	return true;
}

bool S102H5::OpenMetadata(CString _filepath)
{
	return true;
}

S102_FC_BathymetryCoverage* S102H5::GetBathymetryCoverage()
{
	return (S102_FC_BathymetryCoverage*)featureContainer.at(0);
}

void S102H5::DeleteBitmap()
{
	SafeRelease(&pWICBitmap);
}

D2D1_COLOR_F S102H5::getColor(float depth)
{
	if (depth > ENCCommon::DEEP_CONTOUR) {
		return s102Color.DEPDW[0];
	}
	else if (depth > ENCCommon::SAFETY_CONTOUR) {
		return s102Color.DEPMD[0];
	}
	else if (depth > ENCCommon::SHALLOW_CONTOUR) {
		return s102Color.DEPMS[0];
	}
	else if (depth > 0) {
		return s102Color.DEPVS[0];
	}
	else {
		return s102Color.DEPIT[0];
	}

	return D2D1::ColorF(D2D1::ColorF::Black);
}

void S102H5::SetPositive()
{
	if (rootGroup->productSpecification.find("2.0.0") != std::string::npos) {
		auto featureContainer = GetBathymetryCoverage();
		auto featureInstance = featureContainer->GetBathymetryCoverage();
		auto valuesGroup = featureInstance->GetBathymetryCoverage();

		auto width = featureInstance->getNumPointsLongitudinal();
		auto height = featureInstance->getNumPointsLatitudinal();

		auto numPoint = width * height;

		for (int i = 0; i < numPoint; i++) {
			if (valuesGroup->depth[i] != 1000000) {
				valuesGroup->depth[i] = -valuesGroup->depth[i];
			}
		}
	}
}