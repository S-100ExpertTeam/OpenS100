#pragma once
#include "S100_Description.h"
#include <string>

class S100_AreaFillFile
{
public:
	S100_AreaFillFile();
	virtual ~S100_AreaFillFile();

private:
	std::string id;
	S100_Description description;
	std::string fileName;
	std::string fileType;
	std::string fileFormat;

public:
	void Setid(const std::string& value);
	void Setid(const std::wstring& value);
	std::string  Getid();
	std::wstring GetidW();

	void SetDescription(S100_Description* value);
	S100_Description* GetDescription();

	void SetFileName(const std::string& value);
	void SetFileName(const std::wstring& value);
	std::string  GetFileName();
	std::wstring GetFileNameW();

	void SetFileType(const std::string& value);
	void SetFileType(const std::wstring& value);
	std::string  GetFileType();
	std::wstring GetFileTypeW();

	void SetFileFormat(const std::string& value);
	void SetFileFormat(const std::wstring& value);
	std::string  GetFileFormat();
	std::wstring GetFileFormatW();

	void GetContents(pugi::xml_node& node);
};
