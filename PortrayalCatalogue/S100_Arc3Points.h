#pragma once
#include "S100_VectorPoint.h"

class S100_Arc3Points
{
public:
	S100_Arc3Points();
	virtual ~S100_Arc3Points();

private:
	S100_VectorPoint point;
	S100_VectorPoint medianPoint;
	S100_VectorPoint endPoint;

public:
	void SetPoint(S100_VectorPoint* value);
	S100_VectorPoint* GetPoint();

	void SetMedianPoint(S100_VectorPoint* value);
	S100_VectorPoint* GetMedianPoint();

	S100_VectorPoint* GetEndPoint();
	void SetEndPoint(S100_VectorPoint* value);
	void GetContents(pugi::xml_node node);
};