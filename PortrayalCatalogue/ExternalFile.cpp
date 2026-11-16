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

	std::string  ExternalFile::GetFileTypeAsString() { return FileTypeToString(fileType); }
	std::wstring ExternalFile::GetFileType()         { return FileTypeToWString(fileType); }

	std::string  ExternalFile::GetFileFormatAsString()  { return FileFormatToString(fileFormat); }
	std::wstring ExternalFile::GetFileFormatAsWstring() { return FileFormatToWString(fileFormat); }

	Portrayal::FileFormat ExternalFile::GetFileFormat()
	{
		return fileFormat;
	}

	void ExternalFile::SetFileName(const std::string& value)  { FileName = value; }
	void ExternalFile::SetFileName(const std::wstring& value) { FileName = toUtf8(value); }

	void ExternalFile::SetFileType(const std::string& value)  { fileType = StringToFileType(value); }
	void ExternalFile::SetFileType(const std::wstring& value) { fileType = StringToFileType(value); }

	void ExternalFile::SetFileFormat(const std::string& value)  { fileFormat = StringToFileFormat(value); }
	void ExternalFile::SetFileFormat(const std::wstring& value) { fileFormat = StringToFileFormat(value); }

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
				auto _description = new S100_Description();
				_description->GetContents(instruction);
				AddDescription(_description);
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
