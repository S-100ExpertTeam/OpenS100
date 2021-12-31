#pragma once
#include<string>

namespace S100_SVG_D2D1_DLL
{
	class DrawingPattern
	{
	public:
		DrawingPattern();
		virtual ~DrawingPattern();

	public:
		int type;
		std::wstring className; //The color value.
		std::wstring style;

	public:
		void SetType(int value);
		int GetType();

		void SetClass(std::wstring& value);
		const std::wstring& GetClass();

		void SetStyle(std::wstring& value);
		const std::wstring& GetStyle();
	};
}