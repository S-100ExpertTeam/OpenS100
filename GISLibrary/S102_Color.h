#pragma once
class S102_Color
{
public:
	S102_Color();
	virtual ~S102_Color();

public:
	D2D1_COLOR_F DEPDW[3];
	D2D1_COLOR_F DEPMD[3];
	D2D1_COLOR_F DEPMS[3];
	D2D1_COLOR_F DEPVS[3];
	D2D1_COLOR_F DEPIT[3];

private:
	bool bSet = false;
	
public:
	void setColor();
	bool isSet();
};