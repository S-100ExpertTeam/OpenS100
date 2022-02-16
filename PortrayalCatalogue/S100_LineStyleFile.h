#pragma once
#include "S100_Description.h"

#include <string>

class S100_LineStyleFile
{
public:
	S100_LineStyleFile();
	virtual ~S100_LineStyleFile();

private:
	/*
	* Attribute ID value
	*/
	std::wstring id;
	S100_Description description;
	std::wstring fileName;
	std::wstring fileType;
	std::wstring fileFormat;

public:
	void GetContents(pugi::xml_node& node);

	void SetId(std::wstring& value);
	void SetDescription(S100_Description* value);
	void SetFileName(std::wstring& value);
	void SetFileType(std::wstring& value);
	void SetFileFormat(std::wstring& value);

	std::wstring GetId();
	S100_Description* GetDescription();
	std::wstring GetFileName();
	std::wstring GetFileType();
	std::wstring GetFileFormat();
};