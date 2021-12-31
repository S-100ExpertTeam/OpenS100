#pragma once

#include <string>

namespace S100HDF5
{

	class FloatValues
	{
	public:
		FloatValues();
		virtual ~FloatValues();

		std::string GetName();
		float* GetValues();
		int GetValueCount();

		void SetName(std::string name);
		void SetValues(float* values, int count);

	private:
		std::string _Name = "";
		float* _Values = nullptr;
		int _ValueCount = 0;
	};

}