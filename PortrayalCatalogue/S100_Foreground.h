#pragma once
#include <string>
#include <pugixml.hpp>

class S100_Foreground
{
public:
	S100_Foreground();
	virtual ~S100_Foreground();

private:
	std::string token;
	std::string transparency;

public:
	void GetContents(pugi::xml_node node);

	void SetToken(const std::string& value);
	void SetToken(const std::wstring& value);
	std::string  GetToken();
	std::wstring GetTokenW();

	void SetTransparency(const std::string& value);
	void SetTransparency(const std::wstring& value);
	std::string  GetTransparency();
	std::wstring GetTransparencyW();

	void fromDrawingCommand(std::string_view drawingCommand);
};
