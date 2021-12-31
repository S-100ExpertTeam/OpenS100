#include "stdafx.h"
#include "RealSymbol.h"
#include "SVGManager.h"

namespace S100_SVG_D2D1_DLL
{
	RealSymbol::RealSymbol()
	{

	}

	RealSymbol::RealSymbol(std::wstring symName, float x, float y, float scale, float rotation, SVGManager *svgManager)
	{
		SetSymName(symName);
		SetX(x);
		SetY(y);
		SetScale(scale);
		SetRotation(rotation);
		SetSvgManager(svgManager);
	}

	void RealSymbol::SetSymName(std::wstring value) 
	{
		this->symName = value;
	}

	std::wstring RealSymbol::GetSymName()
	{
		return  symName;
	}

	void RealSymbol::SetX(float value) 
	{
		this->x = value;
	}

	float RealSymbol::GetX() 
	{
		return x;
	}

	void RealSymbol::SetY(float value) 
	{
		this->y = value;
	}

	float RealSymbol::GetY() 
	{
		return y;
	}

	void RealSymbol::SetScale(float value) 
	{
		this->scale = value;
	}
	
	float RealSymbol::GetScale() 
	{
		return scale;
	}

	void RealSymbol::SetRotation(float value) 
	{
		this->rotation = value;
	}
	
	float RealSymbol::GetRotation() 
	{
		return rotation;
	}

	void RealSymbol::SetSvgManager(SVGManager* value)
	{
		this->svgManager = value;
	}

	SVGManager* RealSymbol::GetSvgManager()
	{
		return svgManager;
	}
}