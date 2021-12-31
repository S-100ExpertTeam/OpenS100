#pragma once

#include "R_VectorRecord.h"
#include "F_FRID.h"
#include "F_FOID.h"
#include "OrientedCurveRecord.h"

#include <map>

class S101Cell;
class SGeometry;
class SENC_Template;
class R_CurveRecord;
class R_PointRecord;
class F_ATTR;
class F_INAS;
class F_SPAS;
class F_FASC;
class F_MASK;
struct ATTR;
// Feature Type Record
class R_FeatureRecord : Record
{
public:
	R_FeatureRecord();
	virtual ~R_FeatureRecord();

public:
	// 000
	F_FRID m_frid;
	F_FOID m_foid;

	std::list<F_ATTR*> m_attr;
	std::list<F_INAS*> m_inas;
	std::list<F_SPAS*> m_spas;
	std::list<F_FASC*> m_fasc;
	std::list<F_MASK*> m_mask;

	SGeometry *m_geometry = nullptr;

	int m_scaleMin = 0;
	int m_scaleMax = 0;
	int m_bShownAllScale = 0;

	int m_displayPriority = 0;
	int m_viewingGroup = 0;
	int m_displayCategory = 0;
	int m_radarOver = 0;
	bool m_hasTextPlacement = false;
	//	[Text Placement ]
	float* m_textBearing = nullptr;

	double m_depth;

	/*
	0: none
	1: Prohibited Area
	2: Navigational Hazard
	3: Safety Contour
	*/
	int m_alertIndicationType = 0;

	std::list<R_PointRecord*> m_pointList;
	std::list<OrientedCurveRecord> m_curveList;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	void Draw(CDC *pDC, Scaler *scaler, double offset = 0);
	void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	
	/*
	* Drawing Instruction Type
	* 1 : Point Instruction
	* 2 : Curve Instruction
	* 3 : Surface Instruction
	* 4 : Text Instruction
	*/
	void Draw(HDC &hDC, Scaler *scaler, int type, int priority, double offset = 0);
	void CreateCS(S101Cell *cell, CString csName, bool bSENC);
	MBR GetMBR();
	int GetRCID();
	void GetXYPointOfTheVisiblePoints(double &x, double &y);
	int GetAttributeIndex(ATTR* value);
	int GetAssociationCount();
	int GetFeatureAssociationCount();
	int GetInformationAssociationCount();
	int GetNumericCode();

	SGeometry* GetGeometry();
};