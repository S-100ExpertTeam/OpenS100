#pragma once
#include "DrawingPattern.h"

namespace S100_SVG_D2D1_DLL
{
	class Path;
	class BoxLayout : public DrawingPattern
	{
	public:
		BoxLayout();
		virtual ~BoxLayout();

	public:
		std::wstring fill;
		std::wstring x;
		std::wstring y;
		std::wstring height;
		std::wstring width;

	public:
		void SetFill(std::wstring& value);
		const std::wstring& GetFill();

		void SetX(std::wstring& value);
		const std::wstring& GetX();

		void SetY(std::wstring& value);
		const std::wstring& GetY();

		void SetHeight(std::wstring& value);
		const std::wstring& GetHeight();

		void SetWidth(std::wstring& value);
		const std::wstring& GetWidth();

		Path* ToPath();
		std::wstring ToD();

		bool IsFill();
	};
}