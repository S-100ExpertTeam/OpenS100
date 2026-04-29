#pragma once
class ScaleBands
{
public:
	ScaleBands();
	ScaleBands(int minimumScale, int maximumScale);
	virtual ~ScaleBands();

public:
	int minimumScale = -1;
	int optimumScale = -1;
};