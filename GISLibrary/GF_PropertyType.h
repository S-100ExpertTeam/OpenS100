#pragma once

namespace GF
{

	class PropertyType
	{
	public:
		PropertyType();
		virtual ~PropertyType();

	public:
		std::string code;

	public:
		std::string GetCode();
		void SetCode(std::string value);
	};

}