#pragma once

#include <string>

namespace GF
{
	class NamedType
	{
	public:
		NamedType();
		NamedType(std::string code);
		NamedType(const NamedType& other);
		virtual ~NamedType();

	public:
		std::string code = "";

	public:
		virtual NamedType operator=(const NamedType& other);

	public:
		virtual std::string GetCode();
		virtual void SetCode(std::string& value);
		virtual void SetCode(std::wstring value);
	};
}