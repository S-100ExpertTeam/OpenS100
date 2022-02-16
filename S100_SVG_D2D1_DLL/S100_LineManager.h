#pragma once

#include "S100_LineColor.h"
#include "S100_Line.h"
#include "RealLine.h"

#include <string>
#include <unordered_map>

namespace S100_SVG_D2D1_DLL 
{
	class  S100_LineManager
	{
	public:
		S100_LineManager();
		virtual ~S100_LineManager();

	public:
		std::unordered_map<std::wstring, S100_LineColor> colMap;
		std::unordered_map<std::wstring, S100_Line> lineMap;
		std::vector<RealLine> lineList;

	public:
		void GetColorInfoByPugi(std::wstring colorPath);
		void GetLineFiles(std::wstring lineFolderPath);	
	};
}