#pragma once
#include "Field.h"

class F_SEGH : Field
{
public:
	F_SEGH();
	virtual ~F_SEGH();

public:
	/*
	* Interpolation
	* Format		: b11
	* Description	:	{1} - Linear
	*					{2} - Arc3Points
	*					{3} - Geodesic
	*					{4} - Loxodromic
	*					{5} - Elliptical
	*					{6} - Conic
	*					{7} - CircularArcCenterPointWithRadius
	*/
	int m_intp;
	/*
	* Circle or arc
	* Format		: b11
	* Description	:	{1} – segment is a circle
	*					{2} – segment is an arc
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius), otherwise omitted
	*/
	int m_circ;
	/*
	* Coordinate in Y axis
	* Format		: b48
	* Description	:	Y coordinate or latitude of centre
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius), otherwise omitted
	*/
	double m_ycoo;
	/*
	* Coordinate in X axis
	* Format		: b48
	* Description	:	X coordinate or longitude of centre
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius), otherwise omitted
	*/
	double m_xcoo;
	/*
	* Distance
	* Format		: b48
	* Description	:	Radius
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius), otherwise omitted
	*/
	double m_dist;
	/*
	* Distance unit
	* Format		: b11
	* Description	:	{1} Metres
	*					{2} Yards
	*					{3} Kilometres
	*					{4} Statute miles
	*					{5} Nautical miles
	*					{6} Millimetres on screen
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius), otherwise omitted
	*/
	int m_disu;
	/*
	* Start Bearing Angle
	* Format		: b48
	* Description	:	In decimal degrees, range [0, 360.0]
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius) and CIRC = 2 (arc).
	*					Optional if INTP = 7 (CircularArcCenterPointWithRadius) and CIRC = 1 (circle).
	*					Omitted for other values of INTP
	*/
	double m_sbrg;
	/*
	* Angular distance
	* Format		: b48
	* Description	:	In decimal degrees
	*					allowed values -360.0 or +360.0
	*					Required if INTP = 7 (CircularArcCenterPointWithRadius) and CIRC = 2 (arc).
	*					Optional if INTP = 7 (CircularArcCenterPointWithRadius) and CIRC = 1 (circle).
	*					Omitted for other values of INTP
	*/
	double m_angl;

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};