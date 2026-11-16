#include "stdafx.h"
// The portrayal output (drawing instructions) of a dataset. Kept apart from S100SpatialObject.cpp:
// PCOutputSchemaManager builds Direct2D resources.
#include "S100SpatialObject.h"
#include "PCOutputSchemaManager.h"

void S100SpatialObject::InitDrawingInstruction()
{
	ReleaseDrawingInstruction();

	pcManager = new PCOutputSchemaManager();
	pcManagerDeleter = [](PCOutputSchemaManager* manager) { delete manager; };
}

bool S100SpatialObject::OpenOutputXML(std::string path)
{
	InitDrawingInstruction();
	pcManager->displayList->OpenOutputXML(path);
	pcManager->GenerateSENCInstruction(this, GetPC());

	return true;
}
