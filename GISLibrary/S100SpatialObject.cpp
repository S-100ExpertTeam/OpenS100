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

S100SpatialObjectType S100SpatialObject::getS100SpatialObjectType()
{
	return type;
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

GF::ObjectType* S100SpatialObject::GetObjectType(int type, std::string id)
{
	if (type == 1)
	{
		return GetFeatureType(id);
	}
	else if (type == 2)
	{
		return GetInformationType(id);
	}

	return nullptr;
}

int S100SpatialObject::GetFeatureAttributeCount(std::string id)
{
	auto feature = GetFeatureType(id);
	if (feature)
	{
		return feature->GetAttributeCount();
	}

	return 0;
}

std::string S100SpatialObject::GetFeatureAttributeCode(std::string id, int index)
{
	auto feature = GetFeatureType(id);
	if (feature)
	{
		auto attribute = feature->GetAttribute(index);
		if (attribute) {
			return attribute->GetCode();
		}
	}
	
	return std::string("");
}

int S100SpatialObject::GetInformationAttributeCount(std::string id)
{
	auto information = GetInformationType(id);
	if (information)
	{
		return information->GetAttributeCount();
	}

	return 0;
}

std::string S100SpatialObject::GetInformationAttributeCode(std::string id, int index)
{
	auto information = GetInformationType(id);
	if (information)
	{
		auto attribute = information->GetAttribute(index);
		if (attribute) {
			return attribute->GetCode();
		}
	}

	return std::string("");
}

std::string S100SpatialObject::GetObjectAttributeCode(int type, std::string id, int index)
{
	if (type == 1)
	{
		return GetFeatureAttributeCode(id, index);
	}
	else if (type == 2)
	{
		return GetInformationAttributeCode(id, index);
	}

	return "";
}