#include "stdafx.h"
#include "ExternalFile.h"

namespace Portrayal
{
	ExternalFile::ExternalFile()
	{

	}

	ExternalFile::~ExternalFile()
	{

	}

	std::string  ExternalFile::GetFileName()  { return FileName; }
	std::wstring ExternalFile::GetFileNameW() { return toWide(FileName); }

	std::string  ExternalFile::GetFileTypeAsString() { return FileTypeToString(FileType); }
	std::wstring ExternalFile::GetFileType()         { return FileTypeToWString(FileType); }

	std::string  ExternalFile::GetFileFormatAsString()  { return FileFormatToString(FileFormat); }
	std::wstring ExternalFile::GetFileFormatAsWstring() { return FileFormatToWString(FileFormat); }

	Portrayal::FileFormat ExternalFile::GetFileFormat()
	{
		return FileFormat;
	}

	void ExternalFile::SetFileName(const std::string& value)  { FileName = value; }
	void ExternalFile::SetFileName(const std::wstring& value) { FileName = toUtf8(value); }

	void ExternalFile::SetFileType(const std::string& value)  { FileType = StringToFileType(value); }
	void ExternalFile::SetFileType(const std::wstring& value) { FileType = StringToFileType(value); }

	void ExternalFile::SetFileFormat(const std::string& value)  { FileFormat = StringToFileFormat(value); }
	void ExternalFile::SetFileFormat(const std::wstring& value) { FileFormat = StringToFileFormat(value); }

	void ExternalFile::GetContents(pugi::xml_node& node)
	{
		if (!node)
		{
			return;
		}

		auto idAttri = node.attribute("id");
		if (idAttri)
		{
			SetId(std::string(idAttri.value()));
		}

		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();
			if (instructionName == nullptr)
			{
				continue;
			}

			if (!strcmp(instructionName, "description"))
			{
				auto description = new S100_Description();
				description->GetContents(instruction);
				AddDescription(description);
			}
			else if (!strcmp(instructionName, "fileName"))
			{
				SetFileName(std::string(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "fileType"))
			{
				SetFileType(std::string(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "fileFormat"))
			{
				SetFileFormat(std::string(instruction.child_value()));
			}
		}
	}
}
