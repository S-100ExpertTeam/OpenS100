#include "stdafx.h"

#include "S100_LineStyleFile.h"

S100_LineStyleFile::S100_LineStyleFile()
{

}

S100_LineStyleFile::~S100_LineStyleFile()
{

}

void S100_LineStyleFile::GetContents(pugi::xml_node& node)
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

void S100_LineStyleFile::SetId(const std::string& value)  { id = value; }
void S100_LineStyleFile::SetId(const std::wstring& value) { id = toUtf8(value); }
void S100_LineStyleFile::SetDescription(S100_Description* value) { description = *value; }
void S100_LineStyleFile::SetFileName(const std::string& value)   { fileName = value; }
void S100_LineStyleFile::SetFileName(const std::wstring& value)  { fileName = toUtf8(value); }
void S100_LineStyleFile::SetFileType(const std::string& value)   { fileType = value; }
void S100_LineStyleFile::SetFileType(const std::wstring& value)  { fileType = toUtf8(value); }
void S100_LineStyleFile::SetFileFormat(const std::string& value)  { fileFormat = value; }
void S100_LineStyleFile::SetFileFormat(const std::wstring& value) { fileFormat = toUtf8(value); }

std::string  S100_LineStyleFile::GetId()       { return id; }
std::wstring S100_LineStyleFile::GetIdW()      { return toWide(id); }
S100_Description* S100_LineStyleFile::GetDescription() { return &description; }
std::string  S100_LineStyleFile::GetFileName() { return fileName; }
std::wstring S100_LineStyleFile::GetFileNameW(){ return toWide(fileName); }
std::string  S100_LineStyleFile::GetFileType() { return fileType; }
std::wstring S100_LineStyleFile::GetFileTypeW(){ return toWide(fileType); }
std::string  S100_LineStyleFile::GetFileFormat() { return fileFormat; }
std::wstring S100_LineStyleFile::GetFileFormatW(){ return toWide(fileFormat); }
