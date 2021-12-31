#pragma once

#include <vector>

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
		std::vector<ContextParameter*> GetContextParameter();

		void SetContextParameter(ContextParameter* value);
		void SetContextParameter(std::vector<ContextParameter*> value);
	};
}