#pragma once

namespace GF
{

	class PropertyType
	{
	public:
		PropertyType();
		PropertyType(const PropertyType& other);
		virtual ~PropertyType();

	public:
		std::string code;

	public:
		virtual PropertyType operator=(const PropertyType& other);

	public:
		std::string GetCode();
		void SetCode(std::string value);
	};

}