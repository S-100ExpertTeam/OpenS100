#pragma once

class BackgroundSHPFile;
class BackgroundLayer
{
public:
	BackgroundLayer();
	virtual ~BackgroundLayer();

public:
	ID2D1GeometryGroup* geometryGroup = nullptr;

public:
	void CreateGeometry(ID2D1Factory1* pD2Factory, BackgroundSHPFile* shp);
	void Draw(ID2D1DCRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush);
};

