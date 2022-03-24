#pragma once
#include <vector>

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"
namespace Portrayal
{
	class DisplayPlane;
	class DisplayPlanes
	{
	public:
		DisplayPlanes();
		virtual ~DisplayPlanes();

	private:
		std::vector<DisplayPlane*> displayMode;

		std::vector< DisplayPlane*> displayPlanes;
	public:
		int GetCountOfDisplayMode();
		DisplayPlane* GetDisplayMode(int index);
		std::vector<DisplayPlane*> GetDisplayMode();
		void SetDisplayMode(DisplayPlane* value);
		void SetDisplayMode(std::vector<DisplayPlane*> value);

		void GetContents(pugi::xml_node& node);
	};
}