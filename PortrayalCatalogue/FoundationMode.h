#pragma once
#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <unordered_map>
namespace Portrayal
{
	class ViewingGroup;

	class FoundationMode
	{
	public:
		FoundationMode();
		virtual ~FoundationMode();

	private:
		std::unordered_map<int, ViewingGroup*> viewingGroup;

	public:
		void GetContents(pugi::xml_node& node);

		void SetViewingGroup(int index, ViewingGroup* value);
		ViewingGroup* GetViewingGroup(int value);

		bool HasVewingGroup(int value);
	};
}