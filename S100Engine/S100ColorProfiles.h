#pragma once

#include <unordered_map>
#include <string>

class S100ColorProfile;

class S100ColorProfiles
{
public:
	S100ColorProfiles();
	virtual ~S100ColorProfiles();

public:
	std::unordered_map<std::string, S100ColorProfile*> colorProfileMap;

public:
	bool AddColorProfile(const std::string& id, const std::string& path);
	bool AddColorProfile(const std::wstring& id, const std::wstring& path);
};

