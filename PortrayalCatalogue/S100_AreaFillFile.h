#pragma once
#include "S100_Description.h"
#include <string>

class S100_AreaFillFile
{
public:
	S100_AreaFillFile();
	virtual ~S100_AreaFillFile();

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
	void Setid(std::wstring& value);
	std::wstring Getid();

	void SetDescription(S100_Description* value);
	S100_Description* GetDescription();

	void SetFileName(std::wstring& value);
	std::wstring GetFileName();

	void SetFileType(std::wstring& value);
	std::wstring GetFileType();

	void SetFileFormat(std::wstring& value);
	std::wstring GetFileFormat();

	void GetContents(pugi::xml_node& node);
};