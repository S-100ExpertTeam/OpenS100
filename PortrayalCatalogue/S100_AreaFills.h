#pragma once
#include "ExternalFile.h"

#include <unordered_map>
#include <string>

namespace Portrayal
{
	class S100_AreaFills
	{
	public:
		S100_AreaFills();
		virtual ~S100_AreaFills();

	private:
		std::unordered_map<std::wstring, ExternalFile*> areaFillFiles;
		std::vector< ExternalFile*> areaFillFiles_v;

	public:
		void GetContents(pugi::xml_node& node);

		void SetAreaFillFiles(std::wstring key, ExternalFile* value);
		void SetAreaFillFiles(std::unordered_map<std::wstring, ExternalFile*> value);

		ExternalFile* GetAreaFillFiles(std::wstring value);
		std::unordered_map<std::wstring, ExternalFile*> GetAreaFillFiles();
		std::vector< ExternalFile*>* GetAreaFillFilesVecter();
	};
}