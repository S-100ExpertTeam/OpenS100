#pragma once
#include <pugixml.hpp>

#include <string>

class S100_Description
{
public:
	S100_Description();
	virtual ~S100_Description();

public:
	std::string name;
	std::string description;
	std::string language;

private:
	bool on = true;

public:
	void GetContents(pugi::xml_node& node);

	void Setname(const std::string& value);
	void Setname(const std::wstring& value);
	std::string  Getname();
	std::wstring GetnameW();

	void Setdescription(const std::string& value);
	void Setdescription(const std::wstring& value);
	std::string  Getdescription();
	std::wstring GetdescriptionW();

	void Setlanguage(const std::string& value);
	void Setlanguage(const std::wstring& value);
	std::string  Getlanguage();
	std::wstring GetlanguageW();

	void SetOn(bool value);
	bool GetOn();
};
