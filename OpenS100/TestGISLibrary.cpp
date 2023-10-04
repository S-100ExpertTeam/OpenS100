#include "pch.h"
#include "TestGISLibrary.h"
#include "OpenS100.h"
#include "OpenS100View.h"
#include "DockablePaneCurrentSelection.h"
#include "DialogDockCurrentSelection.h"
#include "DialogDockLayerManager.h"

#include "../GISLibrary/R_DSGIR.h"
#include "../GISLibrary/Layer.h"
#include "../GISLibrary/S101Cell.h"
#include "../GISLibrary/S100Layer.h"
#include "../GISLibrary/R_FeatureRecord.h"
#include "../GISLibrary/S101Creator.h"
#include "../GISLibrary/SGeometry.h"

int TestGISLibrary::layerKey = -1;

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

void TestGISLibrary::TestSave()
{
	auto layer = theApp.gisLib->GetLayer(0);
	if (layer)
	{
		// Get S-101 ENC
		auto enc = (S101Cell*)layer->GetSpatialObject();

		enc->Save(L"..\\SampleData\\save.000");
	}
}

void TestGISLibrary::CreateNewLayer()
{
	auto layer = theApp.pView->s101Creator.CreateLayer(L"newENC.000", theApp.gisLib->catalogManager.getFC(), theApp.gisLib->catalogManager.getPC());
	if (layer)
	{
		layerKey = theApp.gisLib->GetLayerManager()->AddLayer(layer);
	}
}

void TestGISLibrary::CreateMemoryLayer()
{
	auto layer = theApp.pView->s101Creator.CreateLayer(L"MemoryLayer.000", theApp.gisLib->catalogManager.getFC(), theApp.gisLib->catalogManager.getPC());
	delete layer;
}

void TestGISLibrary::CopySelectedFeatureToNewLayer()
{
	auto lm = theApp.gisLib->GetLayerManager();
	auto layer = lm->GetLayerByKey(layerKey);
	theApp.pView->s101Creator.SetENC((S101Cell*)layer->GetSpatialObject());

	if (layer)
	{
		auto key = theApp.m_DockablePaneCurrentSelection.pDlg->GetSelectedRecordName();
		if (auto cell = (S101Cell*)theApp.m_pDockablePaneLayerManager.pDlg->GetSelectedCell())
		{
			auto selectedFeature = cell->GetFeatureRecord(key);
			if (selectedFeature)
			{
				auto featureCode = cell->m_dsgir.GetFeatureCode(selectedFeature->GetNumericCode());

				auto feature = theApp.pView->s101Creator.AddFeature(std::wstring(featureCode));
				if (feature)
				{
					if (selectedFeature->geometry)
					{
						unsigned char* buf = nullptr;
						int size = 0;
						if (selectedFeature->geometry->ExportToWkb(&buf, &size))
						{
							auto geom = theApp.pView->s101Creator.SetGeometry(feature, selectedFeature->geometry->GetType(), buf, size);
							
							if (geom)
							{
								geom->CreateD2Geometry(theApp.gisLib->D2.Factory());
								geom->SetMBR();
								theApp.pView->s101Creator.enc->ReMBR();
								theApp.gisLib->GetLayerManager()->ReMBR();
								theApp.gisLib->S101RebuildPortrayal();
							}

							delete[] buf;
							buf = nullptr;
						}
					}
				}
			}
		}
	}
}

void TestGISLibrary::OffFeature()
{
	// Off "DepthArea" feature
	theApp.gisLib->SetFeatureOnOff(L"DepthArea", false);

	// Off "Sounding" feature
	theApp.gisLib->SetFeatureOnOff(L"Sounding", false);

	theApp.pView->MapRefresh();
}

void TestGISLibrary::OpenUpdate()
{
	S101Cell cell;
	cell.Read8211(L"..\\SampleData\\101GB005X01SW.001");
	if (cell.isUpdate()) {
		OutputDebugString(L"Update\n");
	}
	else {
		OutputDebugString(L"Base\n");
	}
	return;
}

void TestGISLibrary::SetCodeNumericCode()
{
	S101Cell cell;
	cell.Read8211(L"..\\SampleData\\101KR004X0000.000");
	auto dsgir = cell.GetDatasetGeneralInformationRecord();
	
	auto oldNumericCode = dsgir->GetFeatureTypeCode(CString(L"LandArea"));
	dsgir->initFeatureTypeCode();
	dsgir->setFeatureTypeCode(100, L"LandArea");
	auto newNemericCode = dsgir->GetFeatureTypeCode(CString(L"LandArea"));
}