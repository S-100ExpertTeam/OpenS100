#pragma once
#include "S100_Description.h"

#include <string>

class S100_LineStyleFile
{
public:
	S100_LineStyleFile();
	virtual ~S100_LineStyleFile();

private:
	std::string id;
	S100_Description description;
	std::string fileName;
	std::string fileType;
	std::string fileFormat;

public:
	void GetContents(pugi::xml_node& node);

	void SetId(const std::string& value);
	void SetId(const std::wstring& value);
	void SetDescription(S100_Description* value);
	void SetFileName(const std::string& value);
	void SetFileName(const std::wstring& value);
	void SetFileType(const std::string& value);
	void SetFileType(const std::wstring& value);
	void SetFileFormat(const std::string& value);
	void SetFileFormat(const std::wstring& value);

	std::string  GetId();
	std::wstring GetIdW();
	S100_Description* GetDescription();
	std::string  GetFileName();
	std::wstring GetFileNameW();
	std::string  GetFileType();
	std::wstring GetFileTypeW();
	std::string  GetFileFormat();
	std::wstring GetFileFormatW();
};
