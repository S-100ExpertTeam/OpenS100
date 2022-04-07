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
		std::wstring FileName = _T("");
		std::wstring FileType = _T("");
		std::wstring FileFormat = _T("");

	public:
		std::wstring GetFileName();
		std::wstring GetFileType();
		std::wstring GetFileFormat();

		void SetFileName(std::wstring value);
		void SetFileType(std::wstring value);
		void SetFileFormat(std::wstring value);

		void GetContents(pugi::xml_node& node);
	};
}

