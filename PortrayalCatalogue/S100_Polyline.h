#pragma once
#include "S100_VectorPoint.h"

#include <list>

class S100_Polyline
{
public:
	S100_Polyline();
	virtual ~S100_Polyline();

private:
	std::list<S100_VectorPoint> points;

public:
	void GetContents(pugi::xml_node node);

	void SetPoints(S100_VectorPoint* value);
	void SetPoints(std::list<S100_VectorPoint> value);

	S100_VectorPoint* GetPoints(int index);
	std::list<S100_VectorPoint> GetPoints();
};