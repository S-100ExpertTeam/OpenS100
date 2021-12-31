#pragma once
#include "DrawingPattern.h"
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class Path : public DrawingPattern
	{
	public:
		Path();
		virtual ~Path();

	public:
		std::wstring d;
	
	public:
		void SetD(std::wstring& value);
		const std::wstring& GetD();
	};
}