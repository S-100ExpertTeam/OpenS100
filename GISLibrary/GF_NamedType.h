#pragma once

#include <string>

namespace GF
{
	class NamedType
	{
	public:
		NamedType();
		virtual ~NamedType();

	public:
		//std::string typeName;
		//std::string definition;
		//bool isAbstract = false;
		std::string code;

	public:
		virtual std::string GetCode();
		virtual void SetCode(std::string& value);
	};
}