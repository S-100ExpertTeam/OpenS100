#pragma once

class S100_CIEXYZ
{
public:
	S100_CIEXYZ();
	S100_CIEXYZ(double X, double Y, double Z);
	virtual ~S100_CIEXYZ();

private:
	double X = 0;
	double Y = 0;
	double Z = 0;

public:
	void setX(double value);
	void setY(double value);
	void setZ(double value);

	double getX();
	double getY();
	double getZ();
};

