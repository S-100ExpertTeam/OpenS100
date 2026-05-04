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
		std::unordered_map<std::string, ExternalFile*> lineStyleFiles;
		std::vector<ExternalFile*> lineStyleFiles_v;

	public:
		void SetLineStyleFiles(const std::string& key, ExternalFile* value);
		void SetLineStyleFiles(const std::wstring& key, ExternalFile* value);
		ExternalFile* GetLineStyleFiles(const std::string& key);
		ExternalFile* GetLineStyleFiles(const std::wstring& key);
		std::unordered_map<std::string, ExternalFile*> GetLineStyleFiles();

		std::vector<ExternalFile*>* GetLineStyleFilesVector();

		void GetContents(pugi::xml_node& node);
	};
}
