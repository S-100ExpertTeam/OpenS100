#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace S100
{
	class Text
	{
	public:

        wstring Value;
		shared_ptr<string> Language;

        void Text::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("language");
            if (idAttri != nullptr)
            {
                Language = make_shared<string>(idAttri.value());
            }
            
            Value = pugi::as_wide(node.child_value());
        }
	};

}