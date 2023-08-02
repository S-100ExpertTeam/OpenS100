#pragma once

#include "MBR.h"

#include <string>

class Scaler;
class Symbol;

struct ID2D1Factory1;

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

public:
	MBR m_mbr;
	std::string id;

public:
	MBR& GetMBRRef();
	std::string GetID();
	std::wstring GetIDAsWString();
	void SetID(std::string value);
	void SetID(std::wstring value);
	void SetID(int value);

public:
	virtual void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0) {};
	virtual void DrawTextInfo(HDC &hDC, Scaler *scaler, double offset = 0) {};

	virtual bool ImportFromWkb(unsigned char* value, int size) { return true; };
	virtual bool ExportToWkb(unsigned char** value, int* size) { return true; };
	bool WriteWkb(std::wstring path);
	virtual int WkbSize() { return 0; };

	virtual void SetMBR() {};
	virtual void CreateD2Geometry(ID2D1Factory1* factory) {};

	// Function for memory release outside of deconstructor
	virtual void Release() {};

	virtual double GetX() { return 0; };
	virtual double GetY() { return 0; };
	virtual double GetX(int index) { return 0; };
	virtual double GetY(int index) { return 0; };

	virtual std::string ToString() { return ""; }

	virtual int getNumPoint() { return 0; };

public:

	/*
	** Function   : Find the distance between the two points.
	** factor   :
				POINT _p1 - first  point
				POINT _p2 - second point
	** Return value : distance between the two points.
	*/
	double GetDistance(POINT _p1, POINT _p2);


	/*
	** Function   : Find the length of the polyline.
	** factor   :
				POINT* _p     - Coordinate arrangement of the polyline.
				int    _count - The number of polyline coordinates.
	** Return value : length of the polyline.
	*/
	double GetDistanceOfPolyline(POINT *_p, int _count);

	/*
	** Function   : Find the line shown on the screen among the polylines.
	** factor   :
				POINT* _p     - Coordinate arrangement of the polyline.
				int    _count - The number of polyline coordinates.
	** Return value : Among the polylines, the line you see on the screen.
	*/
	void GetViewLineInPolyline(POINT *_p, int _count, CRect *viewPort);

	/*
	** Function   : Find the midpoint at the distance of the polyline.
	** factor   :
				POINT* _p     - Coordinate arrangement of the polyline.
				int    _count - The number of polyline coordinates.
	** Return value : The midpoint of the polyline.
	*/
	POINT* GetCenterPointOfPolyline(POINT *_p, int _count, CRect *viewPort);



	/*
	** Function   : Check if the two line segments intersect.
	** factor   :
				POINT _s1     - starting point of LINE 1.
				POINT _e1     - end point of LINE 1.
				POINT _s2     - starting point of LINE 2.
				POINT _e2     - end point of LINE 2.
	** Return value : the two lines intersect.
	*/
	bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);

	/*
	** Function   :
	** factor   : Moving the starting point of the line at the intersection.
				POINT _s1     - starting point of LINE 1.
				POINT _e1     - end point of LINE 1.
				POINT _s2     - starting point of LINE 2.
				POINT _e2     - end point of LINE 2.
	** Return value :
	*/
	void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect *viewPort);


	/*
	** Function   : Find the angle in the positive direction between the two-dimensional vector and the x-axis.
	** factor   :
				POINT _p1 - The starting point of the vector.
				POINT _p2 - The end point of the vector.
	** Return value :  radian, range : 0 ~ 2 * pi
	*/
	double GetAngle(POINT _p1, POINT _p2);


	/*
	** Function   : Find a point (distance) away from the starting point of the line by offset.
	** factor   :
				POINT _p1     - The starting point of the line.
				POINT _p2     - The end point of the vector.
				double offset - offset
	** Return value : The point that is offset from the starting point of the line.
	*/
	POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);
};