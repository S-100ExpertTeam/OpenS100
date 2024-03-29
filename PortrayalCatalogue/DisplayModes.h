#pragma once

#include <vector>

#include <pugixml.hpp>

namespace Portrayal
{
	class DisplayMode;
	class DisplayModes
	{
	public:
		DisplayModes();
		virtual ~DisplayModes();

	private:
		std::vector<DisplayMode*> displayMode;

	public:
		void SetDisplayMode(DisplayMode* value);
		void SetDisplayMode(std::vector<DisplayMode*> value);
		DisplayMode* GetDisplayMode(int index);
		std::vector<DisplayMode*>* GetDisplayModeVector();
		int GetCountOfDisplayMode(); // displayMode

		void GetContents(pugi::xml_node& node);
	};
}