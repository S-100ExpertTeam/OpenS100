#pragma once

#include "SAbstractCurve.h"
#include "SVertex.h"

#include <memory>

class SPoint;

class SCurve : public SAbstractCurve
{
public:
	SCurve();
	SCurve(const SCurve& other);
	virtual ~SCurve();
	
public:
	int m_numPoints = 0;
	SVertex* m_pPoints = nullptr;

	GeoPoint* centerPoint = nullptr;

	bool m_masking = false;
	bool suppress = false;

private:
	//int m_id = 0;

	// The record identifier as a number, read from the text of the identifier once: the text it
	// was read from is kept beside it, and it is read again when the identifier has changed.
	std::string m_rcidText;
	int m_rcid = 0;
	bool m_rcidRead = false;

	// The record identifiers of the points the curve begins and ends at, when it has them.
	static constexpr int noPointID = -1;
	int m_firstPointID = noPointID;
	int m_lastPointID = noPointID;

	// The ends as point objects, made when asked for (GetFirstPoint, GetLastPoint).
	std::unique_ptr<SPoint> m_firstPoint;
	std::unique_ptr<SPoint> m_lastPoint;

	SPoint* EndPoint(std::unique_ptr<SPoint>& kept, int index, int id);

public:
	virtual SCurve operator=(const SCurve& other);

public:
	SGeometryType GetType() override;
	
	int GetNumPoints() const;
	int getNumPoint() const override;

	double GetX(int index) override;
	double GetY(int index) override;
	double GetX() override;
	double GetY() override;
	void Set(int index, double x, double y) override;

	int GetRCID();
	void Init(int size);

	void SetMBR();
	bool ImportFromWkb(unsigned char* value, int size);
	bool ExportToWkb(unsigned char** value, int* size);
	int WkbSize();

	void Projection();
	void SetMultiplicationFactor(int comfX, int comfY);
	void SetRCID(int value);

	bool IsClosed();

	// The record identifiers of the points at the two ends of the curve.
	void SetEndPointIDs(int first, int last);

	// The ends of the curve as points, with their record identifiers. The object belongs to
	// the curve and shows the end as it was when asked for.
	SPoint* GetFirstPoint();
	SPoint* GetLastPoint();

	std::string ToString() override;

	void SetMasking(bool value);
	bool GetMasking();

	void SetSuppress(bool value);
	bool GetSuppress();

	bool isDraw();

	void setCenterPoint() override;
	GeoPoint getCenterPoint();
};
