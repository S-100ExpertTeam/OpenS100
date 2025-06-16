#include "stdafx.h"
#include "GraphicBasePackage_Path.h"

namespace GraphicBasePackage
{
	Path::Path()
	{

	}

	Path::~Path()
	{
		FreeMemory();
	}

	Path& Path::operator=(Path& path)
	{
		FreeMemory();

		for (auto i = path._segment.begin(); i != path._segment.end(); i++)
		{
			_segment.push_back(*i);
		}

		return *this;
	}

	void Path::FreeMemory()
	{
		if (_segment.size() > 0)
		{
			for (auto i = _segment.begin(); i != _segment.end(); i++)
			{
				delete (*i);
				(*i) = nullptr;
			}

			_segment.clear();
		}
	}
}