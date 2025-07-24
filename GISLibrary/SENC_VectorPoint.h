#pragma once

class SENC_VectorPoint
{
public:
	SENC_VectorPoint();
	virtual ~SENC_VectorPoint();

public:
	double x = 0;
	double y = 0;

public:
	double GetX();
	void SetX(double x);

	double GetY();
	void SetY(double y);
};