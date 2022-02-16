#include "stdafx.h"

#include "S100_LineManager.h"
#include "S100_LineStyle.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

namespace S100_SVG_D2D1_DLL
{
	S100_LineManager::S100_LineManager()
	{

	}

	S100_LineManager::~S100_LineManager()
	{

	}

	void S100_LineManager::GetColorInfoByPugi(std::wstring colorPath)
	{
		colorPath = colorPath + _T("ColorProfiles\\colorProfile.xml");
		pugi::xml_document xmldoc;
		auto xmlresult = xmldoc.load_file(colorPath.c_str());

		pugi::xml_node colorsNode = xmldoc.select_node("/colorProfile/colors").node();
		auto colorsName = colorsNode.name();

		for (pugi::xml_node colorNode = colorsNode.first_child(); colorNode; colorNode = colorNode.next_sibling())
		{
			auto colornodeName = colorNode.name();
			if (!strcmp(colornodeName, "color"))
			{
				S100_LineColor col;
	
				for (pugi::xml_attribute attri = colorNode.first_attribute(); attri; attri = attri.next_attribute())
				{
					auto attriName = attri.name();
					if (!strcmp(attriName, "name"))
					{
						col.name = pugi::as_wide(attri.value());
					}
					else if (!strcmp(attriName, "token"))
					{
						col.token = pugi::as_wide(attri.value());
					}
				}

				pugi::xml_node descriptionNode = colorNode.first_child();
				auto descriptionName = descriptionNode.name();

				if (!strcmp(descriptionName, "description"))
				{
					col.description = pugi::as_wide(descriptionNode.child_value());
				}
				std::unordered_map <std::wstring, S100_LineColor>::iterator iter;
				iter = colMap.find(col.token);

				if (iter == colMap.end())
				{
					colMap.insert(std::unordered_map<std::wstring, S100_LineColor>::value_type(col.token, col));
				}
			}
		}

		pugi::xml_node palette = xmldoc.select_node("/colorProfile/palette").node();

		for (pugi::xml_node itemNode = palette.first_child(); itemNode; itemNode = itemNode.next_sibling())
		{
			std::wstring token;
			auto itemName = itemNode.name();
			if (!strcmp(itemName, "item"))
			{
				token = pugi::as_wide(itemNode.first_attribute().value()); //attri:token
				auto srgbNode=itemNode.child("srgb");

				auto redValue =pugi::as_wide(srgbNode.child("red").child_value());
				if (redValue!=L"")
				{
					colMap[token].r = std::stoi(redValue);
				}
				
				auto greenValue = pugi::as_wide(srgbNode.child("green").child_value());
				if (greenValue!=L"")
				{
					colMap[token].g = std::stoi(greenValue);
				}
				
				auto blueValue = pugi::as_wide(srgbNode.child("blue").child_value());
				if (blueValue!=L"")
				{
					colMap[token].b = std::stoi(blueValue);
				}
			
			}
		}
	}

	
	void S100_LineManager::GetLineFiles(std::wstring lineFolderPath) 
	{

		CFileFind  finder;
		std::wstring svgPath = lineFolderPath + _T("LineStyles\\*.xml");
		BOOL bWorking = finder.FindFile(svgPath.c_str());

		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDots() && !finder.IsDirectory())
			{
				std::wstring filepath = lineFolderPath + _T("LineStyles\\") +std::wstring(finder.GetFileName());
				S100_LineStyle pLine;

				std::wstring lineName = finder.GetFileName().Left(finder.GetFileName().GetLength() - 4);
				
				auto item = lineMap.find(lineName);

				if (item == lineMap.end())
				{
					S100_Line line = pLine.ReadLineStyleByPugi(filepath);
					lineMap.insert({ lineName, line });
				}
			}
		}
	}
}