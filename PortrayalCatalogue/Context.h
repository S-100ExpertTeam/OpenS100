#pragma once

#include <vector>
#include <pugixml.hpp>

namespace Portrayal
{
	class ContextParameter;

	class Context
	{
	public:
		Context();
		virtual ~Context();

	private:
		std::vector<ContextParameter*> parameter;

	public:
		int GetCountOfParameter();
		ContextParameter* GetContextParameter(int index);
		ContextParameter* GetContextParameter(std::string id);
		std::vector<ContextParameter*>* GetContextParameter();

		void SetContextParameter(ContextParameter* value);
		void SetContextParameter(std::vector<ContextParameter*> value);
		
		void GetContents(pugi::xml_node& node);
	};
}