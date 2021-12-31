#pragma once
#include "GML_SpatialObject.h"

class MPAMessage :
	public GML_SpatialObject
{
public:
	MPAMessage();
	virtual ~MPAMessage();


	bool Open(CString _filepath);
	void GetENCObject(MSXML2::IXMLDOMNodePtr listPtr);
};
