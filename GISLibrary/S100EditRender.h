#pragma once

class S101Cell;
class SPoint;
namespace s100::render { class IRenderer; }

class S100EditRender
{
public:
	S100EditRender();
	S100EditRender(S101Cell* enc, Scaler* scaler);
	virtual ~S100EditRender();

public:
	S101Cell* enc = nullptr;
	Scaler* scaler = nullptr;
	R_FeatureRecord* feature = nullptr;
	// What the handles are drawn on; set for the frame in hand only.
	s100::render::IRenderer* renderer = nullptr;
	int pointIndex = -1;
	int partIndex = -1;

public:
	void Set(S101Cell* enc, Scaler* scaler, R_FeatureRecord* feature, s100::render::IRenderer* renderer);
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

private:
	double GetSimpleDistance(double x1, double y1, double x2, double y2);
};

