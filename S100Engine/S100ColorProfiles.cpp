#include "stdafx.h"
#include "S100ColorProfiles.h"
#include "S100ColorProfile.h"

#include "../LibMFCUtil/LibMFCUtil.h"

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

bool S100ColorProfiles::AddColorProfile(std::wstring id, std::wstring path)
{
	auto colorProfile = new S100ColorProfile();

	char* cpath = LibMFCUtil::ConvertWCtoC((wchar_t*)path.c_str());
	bool ret = colorProfile->OpenByPugi(cpath);
	delete[] cpath;
	
	if (colorProfileMap.find(id) == colorProfileMap.end())
	{
		colorProfileMap.insert({ id, colorProfile });
		return true;
	}
	
	delete colorProfile;
	return false;
}