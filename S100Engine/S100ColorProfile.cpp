#include "stdafx.h"
#include "S100ColorProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

S100ColorProfile::S100ColorProfile()
{

}

S100ColorProfile::~S100ColorProfile()
{

}

bool S100ColorProfile::OpenByPugi(char *filePath)
{
	pugi::xml_document xmldoc;
	auto result = xmldoc.load_file(filePath);
	auto colorProfileNode = xmldoc.child("colorProfile");

	if (colorProfileNode.empty())
	{
		//OutputDebugString(_T("Failed to Read xml file in S100ColorProfile::Open()\n"));
		return false;
	}

	for (pugi::xml_node instruction = colorProfileNode.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"colors"))
		{
			extractionColors(instruction);
		}
		else if (!strcmp(instructionName, "palette"))
		{
			extractionPalette(instruction);
		}
	}

	if (palette.size() > 0)
	{
		currentPalette = &palette.front();
	}

	return true;
}

D2D1_COLOR_F S100ColorProfile::GetColor(std::wstring _paletteName, std::wstring _token)
{
	D2D1_COLOR_F resultColor = D2D1::ColorF(D2D1::ColorF::Crimson);

	for (auto i = palette.begin(); i != palette.end(); i++) {
		if (i->GetName().compare(_paletteName) == 0) {
			auto item = i->GetItem(_token);
			if (item) {
				return D2D1::ColorF((FLOAT)(item->srgb.red / 255.0), (FLOAT)(item->srgb.green / 255.0), (FLOAT)(item->srgb.blue / 255.0));
			}
		}
	}

	return D2D1::ColorF(D2D1::ColorF::Black);
}

// Current Palette returns the color corresponding to token.
D2D1_COLOR_F S100ColorProfile::GetColor(std::wstring token)
{
	if (currentPalette->IsItem(token))
	{
		auto item = currentPalette->GetItem(token);
		if (nullptr != item)
		{
			return D2D1::ColorF((FLOAT)(item->srgb.red / 255.0), (FLOAT)(item->srgb.green / 255.0), (FLOAT)(item->srgb.blue / 255.0));
		}
	}

	return D2D1::ColorF(D2D1::ColorF::Black);
}

// Save palette pointer to current palette according to palette name
void S100ColorProfile::ChangePalette(std::wstring paletteName) 
{
	for (auto i = palette.begin(); i != palette.end(); i++)
	{
		if (!i->GetName().compare(paletteName))
		{
			currentPalette = &*i;
		}
	}
}

void S100ColorProfile::extractionColors(pugi::xml_node node)
{
	//Add colors' child.
	for (pugi::xml_node instruction=node.first_child(); instruction; instruction =instruction.next_sibling())
	{
		//color
		libS100Engine::Color color;
		if (instruction == nullptr)
		{
			return;
		}
	
		for (pugi::xml_attribute attri = instruction.first_attribute(); attri; attri = attri.next_attribute())
		{
			auto attriName = attri.name();
			if (strcmp(attriName, "name"))
			{
				color.name = pugi::as_wide(attri.value());
				//int i = 0;
			}
			else if (strcmp(attriName, "token"))
			{
				color.token= pugi::as_wide(attri.value());
				//int i = 0;
			}
		}

		auto desctiptionName=instruction.first_child().name();
		if (!strcmp(desctiptionName,"description"))
		{
			color.desctiption =pugi::as_wide(instruction.first_child().child_value());
		}
		colors.colors.push_back(color);
	}
}

void S100ColorProfile::extractionPalette(pugi::xml_node node)
{
	//palette
	Palette pale;
	std::wstring token;
	for (pugi::xml_attribute attri=node.first_attribute(); attri; attri=attri.next_attribute()) 
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"name"))
		{
			pale.SetName(pugi::as_wide(attri.value()));
		}
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		if (node == nullptr)
		{
			return;
		}
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"item"))
		{
			libS100Engine::Item item;
			for (pugi::xml_attribute attri= instruction.first_attribute(); attri; attri=attri.next_attribute())
			{
				auto attriName = attri.name();
				if (!strcmp(attriName,"token"))
				{
					token = pugi::as_wide(attri.value());
					item.token = token;
				}
			}

			for (pugi::xml_node child=instruction.first_child(); child; child=child.next_sibling())
			{
				if (child != nullptr)
				{
					for (pugi::xml_node srgbchild = child.first_child(); srgbchild; srgbchild = srgbchild.next_sibling())
					{
						auto srgbchildName = srgbchild.name();
						if (!strcmp(srgbchildName, "red"))
						{
							std::wstring value = pugi::as_wide(srgbchild.child_value());
							if (value!=L"") 
							{
								item.srgb.red = stoi(value);
							}
						}
						else if (!strcmp(srgbchildName, "green"))
						{
							std::wstring value = pugi::as_wide(srgbchild.child_value());
							if (value != L"")
							{
								item.srgb.green = stoi(value);

							}
						}
						else if (!strcmp(srgbchildName, "blue"))
						{
							std::wstring value = pugi::as_wide(srgbchild.child_value());
							if (value != L"")
							{
								item.srgb.blue = stoi(value);
								int i = 0;
							}
							
						}
					}
				}
			}

			pale.SetItem(token,item);
		}
	}
	palette.push_back(pale);
}


std::vector<CString> S100ColorProfile::Split(CString _data, char parsingChar)
{
	std::vector<CString> result;

	CString data = _data;
	while (data.GetLength() != 0)
	{
		data.TrimLeft(parsingChar);
		int nFind2 = data.Find(parsingChar);
		if (nFind2 == -1)
		{
			result.push_back(data);
			break;
		}
		CString strNum = data.Mid(0, nFind2);
		result.push_back(strNum);
		data = data.Mid(nFind2);
	}
	return result;
}

std::vector<CString> S100ColorProfile::SplitBySpace(CString _data, int _space)
{
	std::vector<CString> result;

	CString data = _data;
	for (int i = 0; i < _data.GetLength(); i += _space)
	{
		CString cs;
		for (int j = 0; j < _space; j++)
		{
			cs.AppendChar(_data[i + j]);
		}
		result.push_back(cs);
	}
	return result;
}

int S100ColorProfile::Conert16to10(char* ch)
{
	int result = strtol(ch, NULL, 16);
	return result;
}

//char* S100ColorProfile::ConvertCstringToChar(CString inCstring)
//{
//	char* result;
//	wchar_t *wchar_str;
//	int char_str_len;
//
//	// 1. CString to wchar * conversion
//	wchar_str = inCstring.GetBuffer(inCstring.GetLength());
//	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
//	//char_str = new char[char_str_len];
//
//	result = (char*)malloc(char_str_len);
//
//	// 2. wchar_t* to char* conversion
//	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, result, char_str_len, 0, 0);
//
//	//delete result;
//	return result;
//}

CString S100ColorProfile::ConvertCharToCString(char* inChar)
{
	return (CString)inChar;
}

std::wstring S100ColorProfile::ConvertCtoWS(char* c)
{
	std::string str(c);
	return std::wstring(str.begin(), str.end()).c_str();
}

char* S100ColorProfile::ConvertWCtoC(wchar_t* str)
{
	//Declare the char* variable to return
	char* pStr = nullptr;

	// Calculates the length of the input wchar_t variable.
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	if (strSize <= 0)
	{
		//OutputDebugString(_T("Failed to WideCharToMultiByte()\n"));
		return nullptr;
	}

	//char* memory allocation
	pStr = new char[strSize];

	//Changing the type.
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}