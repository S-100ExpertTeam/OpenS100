#pragma once
#include <string>
#include <vector>

using namespace std;

namespace S100
{
	class ViewingGroupReference
	{
	public:
		string ref;

        void ViewingGroupReference::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("ref");
            if (idAttri != nullptr)
            {
                ref = idAttri.value();
            }
        }
	};

}
