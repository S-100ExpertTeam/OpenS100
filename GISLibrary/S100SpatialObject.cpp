#include "stdafx.h"
#include "S100SpatialObject.h"
#include "PCOutputSchemaManager.h"
#include "S100Layer.h"

S100SpatialObject::S100SpatialObject()
{

}

S100SpatialObject::~S100SpatialObject()
{
	if (pcManager)
	{
		delete pcManager;
		pcManager = nullptr;
	}
}

void S100SpatialObject::InitDrawingInstruction()
{
	if (pcManager)
	{
		delete pcManager;
	}

	pcManager = new PCOutputSchemaManager();
}

PCOutputSchemaManager* S100SpatialObject::GetPCOutputManager()
{
	return pcManager;
}

int S100SpatialObject::GetProductNumber()
{
	if (m_pLayer)
	{
		auto s100Layer = (S100Layer*)m_pLayer;
		return s100Layer->GetProductNumber();
	}

	return 0;
}

FeatureCatalogue* S100SpatialObject::GetFC()
{
	auto layer = (S100Layer*)GetLayer();
	if (layer)
	{
		return layer->GetFC();
	}

	return nullptr;
}

PortrayalCatalogue* S100SpatialObject::GetPC()
{
	auto layer = (S100Layer*)GetLayer();
	if (layer)
	{
		return layer->GetPC();
	}

	return nullptr;
}

bool S100SpatialObject::OpenOutputXML(std::string path)
{
	InitDrawingInstruction();
	pcManager->displayList->OpenOutputXML(path);
	pcManager->GenerateSENCInstruction(this, GetPC());

	return true;
}