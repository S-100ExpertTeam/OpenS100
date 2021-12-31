#pragma once

class DisplayList;
class CClass_S100_OutputXML
{
public:
	CClass_S100_OutputXML();
	virtual ~CClass_S100_OutputXML();

public:
	static DisplayList* OpenOutputXML(CString path);
	static DisplayList* OpenOutputXMLbypugixml(CString path);
};