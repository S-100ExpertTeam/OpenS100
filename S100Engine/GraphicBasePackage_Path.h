#pragma once
#include "GraphicBasePackage_PathSegment.h"

#include <vector>

namespace GraphicBasePackage
{
	class Path
	{
	public:
		Path();
		virtual ~Path();

	public:
		std::vector<PathSegment*> _segment;

	public:
		void FreeMemory();
		Path& operator=(Path& path);
	};
}