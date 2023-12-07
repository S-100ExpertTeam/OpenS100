#pragma once
#include <string>
#include <vector>
#include "MessageItem.h"

namespace S100
{
	class Messages
	{
	public:
		std::vector<MessageItem*> message;

		void GetContents(pugi::xml_node& node);
	};
}
