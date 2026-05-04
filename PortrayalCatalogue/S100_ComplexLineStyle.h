#pragma once

#include <pugixml.hpp>

#include <string>

class S100_ComplexLineStyle
{
public:
	S100_ComplexLineStyle();
	virtual ~S100_ComplexLineStyle();

private:
	std::string symbolReference;
	std::string scaleFactor;

public:
	void SetSymbolReference(const std::string& value);
	void SetSymbolReference(const std::wstring& value);
	std::string  GetSymbolReference();
	std::wstring GetSymbolReferenceW();

	void SetScaleFactor(const std::string& value);
	void SetScaleFactor(const std::wstring& value);
	std::string  GetScaleFactor();
	std::wstring GetScaleFactorW();

	void GetContents(	pugi::xml_node& node);
};
