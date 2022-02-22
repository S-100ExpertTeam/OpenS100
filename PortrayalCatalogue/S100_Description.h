#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_Description
{
public:
	S100_Description();
	virtual ~S100_Description();

private:
	std::wstring name;
	std::wstring description;
	std::wstring language;

private:
	bool on = true;

public:
	void GetContents(pugi::xml_node& node);

	void Setname(std::wstring& value);
	std::wstring Getname();

	void Setdescription(std::wstring& value);
	std::wstring Getdescription();

	void Setlanguage(std::wstring& value);
	std::wstring Getlanguage();

	void SetOn(bool value);
	bool GetOn();
};