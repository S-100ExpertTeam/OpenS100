#pragma once

#include "ExternalFile.h"

#include <pugixml.hpp>

#include <unordered_map>
#include <string>

class S100_ColorProfile;

namespace Portrayal
{
	class S100_ColorProfiles
	{
	public:
		S100_ColorProfiles();
		virtual ~S100_ColorProfiles();

	private:
		std::unordered_map<std::string, ExternalFile*> colorProfileFiles;
		std::vector<ExternalFile*> colorProfileFiles_v;

	public:
		void GetContents(pugi::xml_node& node);

		std::vector<ExternalFile*>* GetColorProfilesVector();
	};
}
