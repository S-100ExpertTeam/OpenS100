#pragma once

#include "GML_DatasetMetaDataProperty.h"
#include "GML_Feature.h"
#include "GML_Vector.h"
#include "GML_SpatialObject.h"

class Catalog;
class GML_DataSetAttribute;
class MSIMessage : public GML_SpatialObject
{
public:
	MSIMessage();
	virtual ~MSIMessage();

	bool Open(CString _filepath);
	void GetENCObject(MSXML2::IXMLDOMNodePtr listPtr);
};