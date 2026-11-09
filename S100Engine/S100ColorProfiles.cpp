#include "stdafx.h"
#include "S100ColorProfiles.h"
#include "S100ColorProfile.h"


S100ColorProfiles::S100ColorProfiles()
{

}

S100ColorProfiles::~S100ColorProfiles()
{
	for (auto i = colorProfileMap.begin(); i != colorProfileMap.end(); i++)
	{
		delete i->second;
	}
}

bool S100ColorProfiles::AddColorProfile(const std::string& id, const std::string& path)
{
	auto colorProfile = new S100ColorProfile();
	colorProfile->OpenByPugi(path.c_str());

	if (colorProfileMap.find(id) == colorProfileMap.end())
	{
		colorProfileMap.insert({ id, colorProfile });
		return true;
	}

	delete colorProfile;
	return false;
}

bool S100ColorProfiles::AddColorProfile(const std::wstring& id, const std::wstring& path)
{
	return AddColorProfile(s100::text::ToUtf8(id), s100::text::ToUtf8(path));
}
