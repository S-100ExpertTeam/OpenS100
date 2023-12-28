#pragma once

#include <string>

namespace GF
{
	class NamedType
	{
	public:
		NamedType();
		NamedType(const NamedType& other);
		virtual ~NamedType();

	public:
		std::string code = "";

	public:
		virtual std::string GetCode();
		virtual void SetCode(std::string& value);
	};
}