#include "pch.h"
#include "TestGISLibrary.h"
#include "OpenS100.h"

#include "../GISLibrary/R_DSGIR.h"
#include "../GISLibrary/Layer.h"
#include "../GISLibrary/S101Cell.h"

void TestGISLibrary::TestCreateNewCode()
{
	auto layer = theApp.gisLib->GetLayer(0);
	if (layer)
	{
		// Get S-101 ENC
		auto enc = (S101Cell*)layer->GetSpatialObject();

		// Find a feature by code
		CString featureTypeCode = L"Hulk";
		auto numericCode = enc->m_dsgir.m_ftcs->GetCode(featureTypeCode);

		// Failed to find
		if (numericCode == 0)
		{
			// Create or Get a numeric feature code by code
			numericCode = enc->m_dsgir.GetFeatureTypeCode(featureTypeCode);
			CString str;
			str.Format(L"%d\n", numericCode);
			//OutputDebugString(str);
		}
	}
}