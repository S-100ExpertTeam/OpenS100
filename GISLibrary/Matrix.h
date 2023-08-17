#pragma once

#include <vector>

namespace S100
{

	template <typename T>
	class Matrix
	{
	public:
		Matrix();
		virtual ~Matrix();

	public:
		int rows = 0;
		int columns = 0;
		std::vector<T> elements;
	};

	typedef Matrix<int> IntegerMatrix;
	typedef Matrix<double> RealMatrix;
}