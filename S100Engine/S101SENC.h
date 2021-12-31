#pragma once
#include "S100Data.h"
#include <vector>

class S101SENC :
	public S100Data
{
public:
	struct stPoint
	{
		INT32 Y;
		INT32 X;
	};

	struct stCompCurve
	{
		UINT32 RRID; // pointing at the curve.
		BYTE ORNT;
	};

	struct stSurface
	{
		BYTE RRNM;
		UINT32 RRID;
		BYTE ORNT;
		BYTE USAG;
	};

	struct stFeature
	{
		BYTE RRNM;
		UINT32 RRID;
	};

	enum SencType
	{
		Points,
		MultiPoint,
		Curve,
		CompCurve,
		Surface,
		Feature,
	};

public:
	S101SENC();
	virtual ~S101SENC();

public:
	const int SIZEOF_DATALENGTH = sizeof(UINT32);
	const int SIZEOF_DATAID = sizeof(BYTE); 
	const int SIZEOF_ITEMCOUNT = sizeof(UINT32);
	const int SIZEOF_SENCINFO = SIZEOF_DATALENGTH + SIZEOF_DATAID + SIZEOF_ITEMCOUNT;
	const int SIZEOF_INT2DPOINT = sizeof(stPoint);
	const int SIZEOF_COMPCURVEREFINFO = sizeof(stCompCurve);
	const int SIZEOF_SURFACEREFINFO = sizeof(stSurface);
	const int SIZEOF_FEATUREREFINFO = sizeof(stFeature);
	const double COMF = 0.00000001;

	stPoint* s_Point = nullptr;
	int s_PointCount = 0;

	stPoint** s_Curve = nullptr;
	int s_CurveCount = 0;
	int* s_CurveEachCount = nullptr;

	stCompCurve** s_CompCurve = nullptr;
	int s_CompCurveCount = 0;
	int* s_CompCurveEachCount = nullptr;

	stPoint** s_CompCurvePoint = nullptr;
	int* s_CompCurvePointEachCount = nullptr;

	stSurface** s_Surface = nullptr;
	int s_SurfaceCount = 0;
	int* s_SurfaceEachCount = nullptr;

	stFeature* s_Feature = nullptr;
	int s_FeatureCount = 0;

	std::vector<D2D1_POINT_2F>* vecCurve = nullptr;
	std::vector<D2D1_POINT_2F>* vecCompositeCurve = nullptr;
	std::vector<D2D1_POINT_2F>* vecOuterRing = nullptr;

public:
	bool Open(CString path);
	void Release();

	int GetFeatureCount();
	int GetGeometryType(int featureIndex);
	D2D1_POINT_2F GetPoint(int featureID);
	void GetCurve(int featureID, std::vector<D2D1_POINT_2F>& points);
	void GetCompositeCurve(int featureID, std::vector<D2D1_POINT_2F>& points);
	int GetInnerRingCount(int featureID);
	void GetOuterRing(int featureID, std::vector<D2D1_POINT_2F>& points);
	void GetInnerRing(int featureID, int innerRingIndex, std::vector<D2D1_POINT_2F>& points);

private:
	stFeature* GetStFeature(int featureID);
	int GetSpatialReferenceID(int featureID);
	stPoint* GetStPoint(int featureID);
	int GetStCurve(int featureID, stPoint** outputCurve);
	int GetStCompCurve(int featureID, stPoint** outputCompCurve);
};

