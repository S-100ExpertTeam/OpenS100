#pragma once
#include <string>

namespace S100_SVG_D2D1_DLL 
{
	class SVGManager;
	class RealSymbol
	{
	public:
		RealSymbol();
		RealSymbol(std::wstring symName, float x, float y, float scale, float rotation, SVGManager *svgManager);
		virtual ~RealSymbol() {}

	public:
		std::wstring symName;
		float x;
		float y;
		float scale = 1.0f;
		float rotation = 0;
		//SVGManager *svgManager = nullptr;

	public:
		void SetSymName(std::wstring value);
		std::wstring GetSymName();

		void SetX(float value);
		float GetX();

		void SetY(float value);
		float GetY();

		void SetScale(float value);
		float GetScale();

		void SetRotation(float value);
		float GetRotation();

		void SetSvgManager(SVGManager* value);
		SVGManager* GetSvgManager();
	};
}