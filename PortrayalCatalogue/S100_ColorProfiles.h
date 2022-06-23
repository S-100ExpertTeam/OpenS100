#pragma once

#include "ExternalFile.h"

#include "..\\extlibs\pugixml\include\pugixml.hpp"

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
		// key : id
		std::unordered_map<std::wstring, ExternalFile*> colorProfileFiles;
		std::vector<ExternalFile*> colorProfileFiles_v;


	public:
		//Read ColorProfile
		//bool ReadColorProfiles(std::wstring path);

		void GetContents(pugi::xml_node& node);

		//ExternalFile* GetColorProfile();
		//std::unordered_map<std::wstring, ExternalFile*> GetColorProfiles();

		//void SetColorProfile(std::wstring key, S100_ColorProfile* value);
		std::vector<ExternalFile*>* S100_ColorProfiles::GetColorProfilesVector();
	};
}