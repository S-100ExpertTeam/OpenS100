#pragma once

#include <map>

#include "Class_S101_Base.h"
#include "DisplayList.h"
#include "S101SENC.h"
#include "S100PCManager.h"
#include "GraphicBasePackage_ArcByRadius.h"

class S100DrawingUnit;

class S101Dataset
{
public:
	S101Dataset();
	virtual ~S101Dataset();

public:

	std::vector<S100DrawingUnit*>& GetS100DrawingUnit();
	CClass_S101_Base base;
	S101SENC senc;
	CString filePath; // full path
	CString fileName; // filen ame (not extention)
	std::multimap<int, D2D1_POINT_2F> pointMap;
	std::multimap<int, ID2D1PathGeometry*> polyLineMap;
	std::multimap<int, ID2D1PathGeometry*> polygonMap;
	DisplayList* displayList = nullptr;
	std::vector<S100DrawingUnit*> s100DrawingUnit;
	FLOAT xmin = 0;
	FLOAT ymin = 0;
	FLOAT xmax = 0;
	FLOAT ymax = 0;
	int scaleMin = 0;
	int scaleMax = 0;
	int usage = 0;

private:
	ID2D1Geometry * CreateSurfaceGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);

	ID2D1PathGeometry* CreatePolygonGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);

	ID2D1PathGeometry* CreatePolylineGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolylineGeometryFromCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolylineGeometryFromCompositeCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolylineGeometryFromSurfaceFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);

	ID2D1Geometry* CreatePolyLineGeometry(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolyLineGeometryFromCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolyLineGeometryFromCompositeCurveFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);
	ID2D1PathGeometry* CreatePolyLineGeometryFromSurfaceFeature(ID2D1Factory1* pDirect2dFactory, ST_FEATURE_RECORD* pFeatureRecord);

	void GetPoints(ST_FEATURE_RECORD *pFeatureRecord, std::vector<D2D1_POINT_2F>& points);
	void GetPoints(ST_CURVE_RECORD* pST_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points);
	void GetPoints(ST_COMP_CURVE_RECORD* pST_COMP_CURVE_RECORD, std::vector<D2D1_POINT_2F>& points);

	// Create points according to the geometry type of Feature Record
	// Feature record is
	// When it's Point : Point
	// Curve: Curve's center of length
	// In case of a composite curve: Center of run length of the composite curve
	// Surface: Outer Ring's Centroid
	D2D1_POINT_2F CreatePoint(ST_FEATURE_RECORD* pFeatureRecord);
	D2D1_POINT_2F CreatePointFromPointFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord);
	D2D1_POINT_2F CreatePointFromCurveFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord);
	D2D1_POINT_2F CreatePointFromCompositeCurveFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord);
	D2D1_POINT_2F CreatePointFromSurfaceFeatureRecord(ST_FEATURE_RECORD* pFeatureRecord);

	// Create points according to the geometry type of Feature Record
	// Feature record is...
	// When it's Point : Point
	// Curve: Curve's center of length
	// In case of a composite curve: Center of run length of the composite curve
	// Surface: Outer Ring's Centroid
	D2D1_POINT_2F CreatePointFromSENC(int featureID);
	D2D1_POINT_2F CreatePointFromPointFeatureRecordFromSENC(int featureID);
	D2D1_POINT_2F CreatePointFromCurveFeatureRecordFromSENC(int featureID);
	D2D1_POINT_2F CreatePointFromCompositeCurveFeatureRecordFromSENC(int featureID);
	D2D1_POINT_2F CreatePointFromSurfaceFeatureRecordFromSENC(int featureID);

	ID2D1PathGeometry* CreateGeometryFromArcByRadius(ID2D1Factory1* pDirect2dFactory, GraphicBasePackage::ArcByRadius& arcByRadius);

public:
	bool OpenS101(CString _path);
	bool OpenS101SENC(CString _path);
	CString GetFilePath();
	void CreateS100DrawingUnit(ID2D1Factory1* pDirect2dFactory, S100PCManager& s100PCManager);
	void DeleteS100DrawingUnit();
	int GetUsageFromFileName();
};