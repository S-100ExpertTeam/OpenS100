#include "stdafx.h"
#include "BoxLayout.h"
#include "Path.h"

namespace S100_SVG_D2D1_DLL
{
	BoxLayout::BoxLayout()
	{
		type = 10;
	}

	BoxLayout::~BoxLayout()
	{

	}

	void BoxLayout::SetFill(std::wstring& value)
	{
		fill = value;
	}

	const std::wstring& BoxLayout::GetFill()
	{
		return fill;
	}

	void BoxLayout::SetX(std::wstring& value)
	{
		x = value;
	}

	const std::wstring& BoxLayout::GetX()
	{
		return x;
	}

	void BoxLayout::SetY(std::wstring& value)
	{
		y = value;
	}

	const std::wstring& BoxLayout::GetY()
	{
		return y;
	}

	void BoxLayout::SetHeight(std::wstring& value)
	{
		height = value;
	}

	const std::wstring& BoxLayout::GetHeight()
	{
		return height;
	}

	void BoxLayout::SetWidth(std::wstring& value)
	{
		width = value;
	}

	const std::wstring& BoxLayout::GetWidth()
	{
		return width;
	}

	Path* BoxLayout::ToPath()
	{
		auto result = new Path();
		result->SetClass((std::wstring)GetClass());
		result->SetStyle((std::wstring)GetStyle());
		result->SetD(ToD());
		return result;
	}

	std::wstring BoxLayout::ToD()
	{
		std::wstring result = L"M ";

		double xmin = _wtof(GetX().c_str());
		double ymin = _wtof(GetY().c_str());
		double xmax = xmin + _wtof(GetWidth().c_str());
		double ymax = ymin + _wtof(GetHeight().c_str());

		result = L"M " + std::to_wstring(xmin) + L"," + std::to_wstring(ymin) + L" " +
			L"L " + std::to_wstring(xmax) + L"," + std::to_wstring(ymin) + L" " +
			L"L " + std::to_wstring(xmax) + L"," + std::to_wstring(ymax) + L" " +
			L"L " + std::to_wstring(xmin) + L"," + std::to_wstring(ymax) + L" " +
			L"L " + std::to_wstring(xmin) + L"," + std::to_wstring(ymin);

		if (true == IsFill())
		{
			result += L" Z";
		}

		return result;
	}

	bool BoxLayout::IsFill()
	{
		if (0 == wcscmp(GetFill().c_str(), L"none"))
		{
			return false;
		}

		return true;
	}
}