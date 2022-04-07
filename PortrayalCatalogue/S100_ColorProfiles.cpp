#include "stdafx.h"
#include "S100_ColorProfiles.h"
#include "S100_ColorProfile.h"

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
		S100_ColorProfile* colorPath = new S100_ColorProfile();
		colorPath->GetContentsInfo(colorProfile);
		colorProfileFiles[colorPath->Getid()] = colorPath;
		colorProfileFiles_v.push_back(colorPath);
	}
}

bool S100_ColorProfiles::ReadColorProfiles(std::wstring path)
{
	auto colorprofile = colorProfileFiles.begin()->second;
	if (colorprofile == nullptr)
	{
		return false;
	}
	colorprofile->ReadFile(path + L"ColorProfiles\\" + colorprofile->GetfileName());
	return true;
}

S100_ColorProfile* S100_ColorProfiles::GetColorProfile()
{
	if (colorProfileFiles.size() >= 1)
	{
		return colorProfileFiles.begin()->second;
	}

	return nullptr;
}


std::unordered_map<std::wstring, S100_ColorProfile*>  S100_ColorProfiles::GetColorProfiles()
{
	return colorProfileFiles;
}

void S100_ColorProfiles::SetColorProfile(std::wstring key, S100_ColorProfile* value)
{
	colorProfileFiles[key] = value;
}

std::vector< S100_ColorProfile*>* S100_ColorProfiles::GetColorProfilesVector()
{
	return &colorProfileFiles_v;
}

