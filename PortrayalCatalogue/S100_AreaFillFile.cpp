#include "stdafx.h"
#include "S100_AreaFillFile.h"

S100_AreaFillFile::S100_AreaFillFile()
{

}

S100_AreaFillFile::~S100_AreaFillFile()
{

}

void S100_AreaFillFile::Setid(const std::string& value)  { id = value; }
void S100_AreaFillFile::Setid(const std::wstring& value) { id = toUtf8(value); }
std::string  S100_AreaFillFile::Getid()  { return id; }
std::wstring S100_AreaFillFile::GetidW() { return toWide(id); }

void S100_AreaFillFile::SetDescription(S100_Description* value) { description = *value; }
S100_Description* S100_AreaFillFile::GetDescription() { return &description; }

void S100_AreaFillFile::SetFileName(const std::string& value)  { fileName = value; }
void S100_AreaFillFile::SetFileName(const std::wstring& value) { fileName = toUtf8(value); }
std::string  S100_AreaFillFile::GetFileName()  { return fileName; }
std::wstring S100_AreaFillFile::GetFileNameW() { return toWide(fileName); }

void S100_AreaFillFile::SetFileType(const std::string& value)  { fileType = value; }
void S100_AreaFillFile::SetFileType(const std::wstring& value) { fileType = toUtf8(value); }
std::string  S100_AreaFillFile::GetFileType()  { return fileType; }
std::wstring S100_AreaFillFile::GetFileTypeW() { return toWide(fileType); }

void S100_AreaFillFile::SetFileFormat(const std::string& value)  { fileFormat = value; }
void S100_AreaFillFile::SetFileFormat(const std::wstring& value) { fileFormat = toUtf8(value); }
std::string  S100_AreaFillFile::GetFileFormat()  { return fileFormat; }
std::wstring S100_AreaFillFile::GetFileFormatW() { return toWide(fileFormat); }

void S100_AreaFillFile::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto idvalue = node.attribute("id").value();
	if (idvalue)
	{
		id = idvalue;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "description"))
		{
			description.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "fileName"))
		{
			fileName = instruction.child_value();
		}
		else if (!strcmp(instructionName, "fileType"))
		{
			fileType = instruction.child_value();
		}
		else if (!strcmp(instructionName, "fileFormat"))
		{
			fileFormat = instruction.child_value();
		}
	}
}
