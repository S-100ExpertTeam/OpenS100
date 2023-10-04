#pragma once

namespace GF
{

	class PropertyType
	{
	public:
		PropertyType();
		~PropertyType();

	public:
		std::string code;

	public:
		std::string GetCode();
		void SetCode(std::string value);
	};

}