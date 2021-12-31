#pragma once
namespace ClipperLib
{
	class PolyTree;
}

class GeoPoint;
class Scaler;
class ClipperUtil
{
public:
	static ClipperLib::PolyTree* ClipPolyline(GeoPoint* _p, int _cnt, Scaler* scaler);
};