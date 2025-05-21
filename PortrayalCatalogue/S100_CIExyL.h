#pragma once
class S100_CIExyL
{
public:
	S100_CIExyL() = default;
	S100_CIExyL(double x, double y, double L);
	virtual ~S100_CIExyL();

private:
	double x = 0;
	double y = 0;
	double L = 0;

public:
	void setX(double value);
	void setY(double value);
	void setL(double value);

	double getX();
	double getY();
	double getL();
};

