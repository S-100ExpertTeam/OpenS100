#pragma once
#include "ExternalFile.h"

#include <unordered_map>
#include <string>

namespace Portrayal
{
	class S100_LineStyles
	{
	public:
		S100_LineStyles();
		virtual ~S100_LineStyles();

	private:
		std::unordered_map<std::wstring, ExternalFile*> lineStyleFiles;
		std::vector< ExternalFile*> lineStyleFiles_v;

	public:
		void SetLineStyleFiles(std::wstring& key, ExternalFile* value);
		ExternalFile* GetLineStyleFiles(std::wstring key);
		std::unordered_map<std::wstring, ExternalFile*> GetLineStyleFiles();

		std::vector< ExternalFile*>* GetLineStyleFilesVector();

		void GetContents(pugi::xml_node& node);
	};
}