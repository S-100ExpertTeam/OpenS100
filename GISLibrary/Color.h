#pragma once

namespace nmColorMap {

	class Color
	{
	public:
		Color();
		Color(int _r, int _g, int _b);
		Color(COLORREF _color);
		virtual ~Color();

	public:
		COLORREF color;

	public:
		int R();
		int G();
		int B();
		void Get(int& _r, int& _g, int& _b);
		void SetR(int _r);
		void SetG(int _g);
		void SetB(int _b);
		void Set(int _r, int _g, int _b);
		void Set(COLORREF _color);
	};
}