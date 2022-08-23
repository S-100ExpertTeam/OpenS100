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
struct SPAS;

// Feature Type Record
class R_FeatureRecord : Record
{
public:
	R_FeatureRecord();
	virtual ~R_FeatureRecord();

public:
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

	// -1은 유효하지 않은 Priority
	// No geometry이거나 Drawing Instruction이 없는 경우 -1
	// Feature와 같은 Primitive의 Drawing Instruction의 우선순위만 계산
	int minimumDisplayPriority = -1;
	int maximumDisplayPriority = -1;

	bool m_hasTextPlacement = false;

	//	[Text Placement ]
	float* m_textBearing = nullptr;

	/*
	0: none
	1: Prohibited Area
	2: Navigational Hazard
	3: Safety Contour
	*/
	int m_alertIndicationType = 0;

	std::list<OrientedCurveRecord> m_curveList;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);

	void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	
	MBR GetMBR();
	int GetRCID();
	int GetAssociationCount();
	int GetFeatureAssociationCount();
	int GetInformationAssociationCount();
	int GetNumericCode();

	int MinimumDisplayPriority();
	void MinimumDisplayPriority(int value);

	int MaximumDisplayPriority();
	void MaximumDisplayPriority(int value);

	// 0 : No geometry
	// 110 ~ 130 : normal
	// -1 : error
	int GetSPASRCNM();
	int GetSPASCount();

	std::vector<ATTR*> GetAllAttributes();

	std::vector<ATTR*> GetRootAttributes();
	std::vector<ATTR*> GetRootAttributes(int numericCode);

	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR);
	std::vector<ATTR*> GetChildAttributes(ATTR* parentATTR, int numericCode);

	int GetAttributeIndex(ATTR* attr);

	SGeometry* GetGeometry();

	SPAS* GetSPAS();
};