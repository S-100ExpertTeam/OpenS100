#pragma once
#include "figure.h"
#include "Line.h"
#include "Circle.h"
#include "PivotPoint.h"
#include "SVGGeometry.h"

#include <vector>
#include <string>

struct ID2D1Factory1;
struct ID2D1PathGeometry;
class SVGReader
{
public:
	SVGReader();
	SVGReader(char* path);
	virtual ~SVGReader();

	bool OpenByPugi(char* path);
	void Close();

	void CreateSVGGeometry(ID2D1Factory1* m_pDirect2dFactory);
	SVGGeometry CreateSVGGeometryFromLine(ID2D1Factory1* m_pDirect2dFactory, libS100Engine::Line* line);
	SVGGeometry CreateSVGGeometryFromCircle(libS100Engine::Circle* circle);

	std::vector<libS100Engine::Figure*> figures;
	std::wstring name;

	// Resources used for svg drawing using direct2d.
	std::vector<SVGGeometry> geometry;
	
	// svgBox 
	float bounding_x = 0;
	float bounding_y = 0;
	float bounding_width = 0;
	float bounding_height = 0;
};