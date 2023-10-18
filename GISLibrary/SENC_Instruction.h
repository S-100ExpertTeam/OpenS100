#pragma once

#include "../GeoMetryLibrary/Scaler.h"

#include <polyclipping/clipper.hpp>

#include <list>

class R_FeatureRecord;
class SENC_TextInstruction;
class SSurface;
class Symbol;
class SENC_SymbolFill;
class SENC_LineStyle;
class SENC_ArcByRadius;
class SENC_SpatialReference;
class S100_Instruction;
class S100SpatialObject;

class SENC_Instruction
{
public:
	SENC_Instruction();
	virtual ~SENC_Instruction();

public:
	/*
	*  Type Of Instruction
	*  0 : Null Instruction
	*  1 : Point Instruction
	*  2 : Line Instruction
	*  3 : Area Instruction
	** 4 : Coverage Instruction
	*  5 : Text Instruction
	** 6 : Augmented Point
	** 7 : Augmented Ray
	** 8 : Augmented Path
	** 9 : Augmented Area
	*/
	unsigned type;

	//////////////////////////////////////////////////////////
	// Instruction Information

	// Feature ID
	int featureReference;

	// FeatureType code (for performance)
	std::wstring code;
	
	// Vector ID
	std::list<SENC_SpatialReference*> spatialReference;
	
	/*
	* display group
	* 10001 ~ 20000   :		display base 
	* 20001 ~ 30000   :		standard
	* 30001 ~ 40000   :		others(all)
	*/
	int viewingGroup;
	/*
	* display plane
	* 0 : UNDERRADAR  
	* 1 : Overradar
	*/
	int displayPlane;
	/*
	* display priority
	* 10 ~ 90 (term unit is 10 in ENC Object)
	*/
	int drawingPriority;
	//int drawingPriority_initValue;

	// Scale Minimum
	unsigned scaleMinimum;
	// Scale Maximum
	unsigned scaleMaximum;

	bool suppressedInstance;

	// For Drawing
	GF::FeatureType* fr = nullptr;

public:
	int FeatureReference();
	int DrawingPriority();

	// Ensure that Instruction is referring to the Space Reference separately (if referring separately, it should be drawn in the geometry of the information in the corresponding Space Reference).
	bool HasSpatialReference();

	virtual void GetDrawPoints(Scaler *scaler, std::list<D2D1_POINT_2F> &points){};
	virtual void GetDrawPointsDynamic(Scaler *scaler, std::list<D2D1_POINT_2F> &points){};

	virtual void DrawInstruction(
		ID2D1DCRenderTarget* rt, 
		ID2D1SolidColorBrush* brush, 
		std::vector<ID2D1StrokeStyle1*>* strokeGroup, 
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr) {};
	
	virtual void DrawInstruction(
		ID2D1DCRenderTarget* rt, 
		ID2D1Factory1* pDirect2dFactory, 
		ID2D1SolidColorBrush* brush, 
		std::vector<ID2D1StrokeStyle1*>* strokeGroup, 
		Scaler *scaler, 
		PortrayalCatalogue* pc = nullptr) {};
	
	virtual void DrawInstruction(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler){};

	virtual void ChangePallete(PortrayalCatalogue *pc) {};

	virtual void FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output) {};

	static SENC_Instruction* S1002SENC(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output, S100SpatialObject* so);

	void CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler);

	/*
	** function   : Clips the given AREA to fit the screen area.
	** factor   :
	ENCArea* _polygon  - cliping AREA
	CRect*   _viewPort - screen area
	** return value :
	NULL  - If the screen area and AREA do not overlap at all,
	!NULL - As a result of clipping, it may be a multi-part.
	*/
	ClipperLib::Paths ClipSurface(SSurface *_polygon, CRect *_viewPort);


	/*
	** function   : Find the center of gravity of a given polygon.
	** factor   :
	POINT* _p     - The arrangement with the coordinates of polygon.
	int    _count - The number of polygon coordinates.
	** return value : center point
	*/
	POINT CalculateCenterOfGravityOfSurface(POINT *_p, int _count);


	/*
	** function   : Find the center of the polygon.
	** factor   :
	POINT* _p     - The arrangement with the coordinates of polygon.
	int    _count - The number of polygon coordinates.
	** return value : center point
	*/
	POINT CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon);


	/*
	** function   : Find the distance between the two points.
	** factor   :
	POINT _p1 - first point
	POINT _p2 - second point
	** return value : The distance between the two points
	*/
	double GetDistance(POINT _p1, POINT _p2);

	/*
	** function   :
	** factor   : Move the starting point of the line from the intersection.
	POINT _s1     - starting point of LINE 1.
	POINT _e1     - end point of LINE 1.
	POINT _s2     - starting point of LINE 2.
	POINT _e2     - end point of LINE 2.
	** return value :
	*/
	void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort);


	/*
	** function   : Find the angle in the positive direction between the two-dimensional vector and the x-axis.
	** factor   :
	POINT _p1 - starting point of the vector.
	POINT _p2 - end point of the vector.
	** return value :  radian, range : 0 ~ 2 * pi
	*/
	double GetAngle(POINT _p1, POINT _p2);


	/*
	** function   : Find a point (distance) away from the starting point of the line by offset.
	** factor   :
	POINT _p1     - starting point fo the line
	POINT _p2     - end point fo the line
	double offset - offset
	** return value : The point that is offset from the starting point of the line.
	*/
	POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);
	Gdiplus::PointF GetOffsetPointFOnLine(POINT _p1, POINT _p2, double offset);
	Gdiplus::PointF GetOffsetPointFOnLine(POINT _p1, POINT _p2, double offset, double gapX, double gapY);
};

