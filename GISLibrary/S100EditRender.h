#pragma once

class S101Cell;
class SPoint;

class S100EditRender
{
public:
	S100EditRender();
	S100EditRender(S101Cell* enc);
	virtual ~S100EditRender();

public:
	S101Cell* enc = nullptr;
	R_FeatureRecord* feature = nullptr;
	int pointIndex = -1;
	int partIndex = -1;

public:
	void Set(S101Cell* enc, R_FeatureRecord* feature);
	void ShowPoint();
	void ShowPoint(SPoint* geom);
	void ShowPoint(SMultiPoint* geom);
	void ShowPoint(SCurve* geom);
	void ShowPoint(SCompositeCurve* geom);
	void ShowPoint(SSurface* geom);

	void SelectByScreen(int sx, int sy);
	void SelectByScreen(int sx, int sy, SPoint* geom);
	void SelectByScreen(int sx, int sy, SMultiPoint* geom);
	void SelectByScreen(int sx, int sy, SCurve* geom);
	void SelectByScreen(int sx, int sy, SCompositeCurve* geom);
	void SelectByScreen(int sx, int sy, SSurface* geom);

	void UpdatePoint(int sx, int sy);
	void UpdatePoint(int sx, int sy, SPoint* geom);
	void UpdatePoint(int sx, int sy, SMultiPoint* geom);
	void UpdatePoint(int sx, int sy, SCurve* geom);
	void UpdatePoint(int sx, int sy, SCompositeCurve* geom);
	void UpdatePoint(int sx, int sy, SSurface* geom);

private:
	double GetSimpleDistance(double x1, double y1, double x2, double y2);
};

