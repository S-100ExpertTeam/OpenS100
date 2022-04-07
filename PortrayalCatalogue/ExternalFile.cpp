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

	std::wstring ExternalFile::GetFileName()
	{
		return FileName;
	}

	std::wstring ExternalFile::GetFileType()
	{
		return FileType;
	}

	std::wstring ExternalFile::GetFileFormat()
	{
		return FileFormat;
	}

	void ExternalFile::SetFileName(std::wstring value)
	{
		FileName = value;
	}

	void ExternalFile::SetFileType(std::wstring value)
	{
		FileType = value;
	}

	void ExternalFile::SetFileFormat(std::wstring value)
	{
		FileFormat = value;
	}

	void ExternalFile::GetContents(pugi::xml_node& node)
	{
		if (!node)
		{
			return;
		}


		auto idAttri = node.attribute("id");
		if (idAttri)
		{
			SetId(pugi::as_wide(idAttri.value()));
		}

		S100_Description* description = new S100_Description();
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();
			if (instructionName == nullptr)
			{
				continue;
			}

			if (!strcmp(instructionName, "description"))
			{
				description = new S100_Description();
				description->GetContents(instruction);
				AddDescription(description);
			}
			else if (!strcmp(instructionName, "fileName"))
			{
				SetFileName(pugi::as_wide(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "fileType"))
			{
				SetFileType(pugi::as_wide(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "fileFormat"))
			{
				SetFileFormat(pugi::as_wide(instruction.child_value()));
			}
		}
	}
}
