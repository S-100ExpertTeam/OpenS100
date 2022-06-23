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
	std::unordered_map<std::wstring, S100ColorProfile*> colorProfileMap;

public:
	bool AddColorProfile(std::wstring id, std::wstring path);
};

