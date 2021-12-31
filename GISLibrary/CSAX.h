#pragma once

struct CSAX
{
public:
	/*
	* Axis Type
	* Format		: b11
	* see table
	*/
	int m_axty;

	/*
	* Axis Unit of Measure
	* Format		: b11
	* {1} - Degree
	* {2} - Grad
	* {3} - Radian
	* {4} - Metre
	* {5} - International foot
	* {6} - US survey foot
	*/
	int m_axum;

public:
	static int GetSize();
};