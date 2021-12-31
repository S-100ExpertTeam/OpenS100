#pragma once
#include <vector>

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

	public:
		int GetCountOfDisplayMode();
		DisplayPlane* GetDisplayMode(int index);
		std::vector<DisplayPlane*> GetDisplayMode();
		void SetDisplayMode(DisplayPlane* value);
		void SetDisplayMode(std::vector<DisplayPlane*> value);
	};
}