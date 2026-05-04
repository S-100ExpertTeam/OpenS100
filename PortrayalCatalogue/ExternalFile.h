#pragma once

#include "CatalogItem.h"
#include "Enum_FileType.h"
#include "Enum_FileFormat.h"

namespace Portrayal
{
	class ExternalFile :
		public CatalogItem
	{
	public:
		ExternalFile();
		virtual ~ExternalFile();

	private:
		std::string FileName;
		FileType FileType = FileType::none;
		FileFormat FileFormat = FileFormat::none;

	public:
		std::string  GetFileName();
		std::wstring GetFileNameW();
		std::string  GetFileTypeAsString();
		std::wstring GetFileType();
		std::string  GetFileFormatAsString();
		std::wstring GetFileFormatAsWstring();
		Portrayal::FileFormat GetFileFormat();

		void SetFileName(const std::string& value);
		void SetFileName(const std::wstring& value);
		void SetFileType(const std::string& value);
		void SetFileType(const std::wstring& value);
		void SetFileFormat(const std::string& value);
		void SetFileFormat(const std::wstring& value);

		void GetContents(pugi::xml_node& node);
	};
}
