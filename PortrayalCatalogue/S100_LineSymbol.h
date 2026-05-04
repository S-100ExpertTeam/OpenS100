#pragma once
#include <string>

#include <pugixml.hpp>

class S100_LineSymbol
{
public:
	S100_LineSymbol();
	virtual ~S100_LineSymbol();

public:
	// attribute
	std::string reference;
	std::string rotation;
	std::string scaleFactor;
	std::string crsType;

	// element
	std::string position;

public:
	void GetContents(pugi::xml_node& node);

	void SetReference(const std::string& value);
	void SetReference(const std::wstring& value);
	void SetRotation(const std::string& value);
	void SetRotation(const std::wstring& value);
	void SetScaleFactor(const std::string& value);
	void SetScaleFactor(const std::wstring& value);
	void SetCrsType(const std::string& value);
	void SetCrsType(const std::wstring& value);
	void SetPosition(const std::string& value);
	void SetPosition(const std::wstring& value);

	std::string  GetReference();
	std::wstring GetReferenceW();
	std::string  GetRotation();
	std::wstring GetRotationW();
	std::string  GetScaleFactor();
	std::wstring GetScaleFactorW();
	std::string  GetCrsType();
	std::wstring GetCrsTypeW();
	std::string  GetPosition();
	std::wstring GetPositionW();
};
