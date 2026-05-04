#include "stdafx.h"
#include "S100_ColorProfiles.h"
#include "S100_ColorProfile.h"

namespace Portrayal
{
	S100_ColorProfiles::S100_ColorProfiles()
	{

	}

	S100_ColorProfiles::~S100_ColorProfiles()
	{
		for (auto itor = colorProfileFiles.begin(); itor != colorProfileFiles.end(); itor++)
		{
			delete itor->second;
		}
	}

	void S100_ColorProfiles::GetContents(pugi::xml_node& node)
	{
		auto colorProfile = node.first_child();
		const pugi::char_t* instructionName = colorProfile.name();

		if (!strcmp(instructionName, "colorProfile"))
		{
			ExternalFile* externalFile = new ExternalFile();
			externalFile->GetContents(colorProfile);

			if (colorProfileFiles.find(externalFile->GetId()) == colorProfileFiles.end())
			{
				colorProfileFiles_v.push_back(externalFile);
				colorProfileFiles.insert({ externalFile->GetId(), externalFile });
			}
			else
			{
				delete externalFile;
			}
		}
	}

	std::vector<ExternalFile*>* S100_ColorProfiles::GetColorProfilesVector()
	{
		return &colorProfileFiles_v;
	}
}
