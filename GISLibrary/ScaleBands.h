#pragma once
class ScaleBands
{
public:
	ScaleBands();
	ScaleBands(int maximumScale, int minimumScale);
	virtual ~ScaleBands();

public:
	int minimumScale = -1;
	int maximumScale = -1;
};