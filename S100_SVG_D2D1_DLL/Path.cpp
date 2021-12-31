#include "stdafx.h"
#include "Path.h"

namespace S100_SVG_D2D1_DLL
{
	Path::Path()
	{
		type = 1;
	}

	Path::~Path()
	{

	}

	void Path::SetD(std::wstring& value)
	{
		d = value;
	}

	const std::wstring& Path::GetD()
	{
		return d;
	}
}