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
	void ShowPoint(LayerManager* layerManager);
	void ShowPoint(LayerManager* layerManager, SPoint* geom);
	void ShowPoint(LayerManager* layerManager, SMultiPoint* geom);
	void ShowPoint(LayerManager* layerManager, SCurve* geom);
	void ShowPoint(LayerManager* layerManager, SCompositeCurve* geom);
	void ShowPoint(LayerManager* layerManager, SSurface* geom);

	void SelectByScreen(int sx, int sy, LayerManager* layerManager);
	void SelectByScreen(int sx, int sy, LayerManager* layerManager, SPoint* geom);
	void SelectByScreen(int sx, int sy, LayerManager* layerManager, SMultiPoint* geom);
	void SelectByScreen(int sx, int sy, LayerManager* layerManager, SCurve* geom);
	void SelectByScreen(int sx, int sy, LayerManager* layerManager, SCompositeCurve* geom);
	void SelectByScreen(int sx, int sy, LayerManager* layerManager, SSurface* geom);

	void UpdatePoint(int sx, int sy, LayerManager* layerManager);
	void UpdatePoint(int sx, int sy, LayerManager* layerManager, SPoint* geom);
	void UpdatePoint(int sx, int sy, LayerManager* layerManager, SMultiPoint* geom);
	void UpdatePoint(int sx, int sy, LayerManager* layerManager, SCurve* geom);
	void UpdatePoint(int sx, int sy, LayerManager* layerManager, SCompositeCurve* geom);
	void UpdatePoint(int sx, int sy, LayerManager* layerManager, SSurface* geom);

private:
	double GetSimpleDistance(double x1, double y1, double x2, double y2);
};

